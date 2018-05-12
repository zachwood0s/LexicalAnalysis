#include <stdlib.h>
#include "parser.h"
#include "lexar.h"


Parser::Parser(Lexar* lexar){
    this->lexar = lexar;
}

void Parser::ConsumeError(LexicalTokenType type){
    printf("ERROR at line: %d col: %d\n in file %s\n", lexar->lineNumber, lexar->columnNumber, lexar->fileName.c_str());
    printf("Expected type of '%s', got type of '%s'\n",
            lexicalTokenNames[type],
            lexicalTokenNames[currentToken.type]);
    throw "Consuming failed";
}

void Parser::Consume(LexicalTokenType type){
    if(currentToken.type == type){
        currentToken = lexar->NextToken();
    }
    else{
        ConsumeError(type);
    }
}

bool Parser::Parse(){
    try{
        currentToken = lexar->NextToken();
        Program();
        Consume(EOI);
        return true;
    } catch(const char * msg){
        printf("%s\nExiting\n", msg);
    }
    return false;
}

/************************/
/*      Main Program    */
/************************/

std::unique_ptr<AST> Parser::Program(){
    ProgramHeader();
    auto mainBlock = Block();
    Consume(DOT);
    return std::move(mainBlock);
}

std::string Parser::ProgramHeader(){
    Consume(KW_PROGRAM);
    auto programName = currentToken.identifierName;
    Consume(IDENTIFIER);
    Consume(SEMICOLON);
    return programName;
}

std::unique_ptr<AST> Parser::Block(){
    DeclarationPart();
    StatementPart();
}

std::unique_ptr<AST> Parser::DeclarationPart(){
    switch(currentToken.type){
        case KW_VAR:
            {
                VariableDeclaration();
                DeclarationPart();
                break;
            }
        case KW_CONST:
            {
                ConstantDeclaration();
                DeclarationPart();
                break;
            }
        case KW_PROCEDURE:
            {
                ProcedureDeclaration();
                DeclarationPart();
                break;
            }
        case KW_FUNCTION:
            {
                FunctionDeclaration();
                DeclarationPart();
                break;
            }
        default: break;
    }
}

std::unique_ptr<AST> Parser::VariableDeclaration(){
    Consume(KW_VAR);
    VariableDeclarationPart();
    VariableDeclarationPrime();
}

std::unique_ptr<AST> Parser::VariableDeclarationPrime(){
    if(currentToken.type == IDENTIFIER){
        VariableDeclarationPart();
        VariableDeclarationPrime();
    }
}

std::unique_ptr<AST> Parser::VariableDeclarationPart(){
    IdentifierList();
    Consume(COLON);
    Type();
    Consume(SEMICOLON);
}

std::unique_ptr<AST> Parser::ConstantDeclaration(){
    Consume(KW_CONST);
    ConstantDeclarationPart();
    ConstantDeclarationPrime();
}

std::unique_ptr<AST> Parser::ConstantDeclarationPrime(){
    if(currentToken.type == IDENTIFIER){
        ConstantDeclarationPart();
        ConstantDeclarationPrime();
    } 
}

std::unique_ptr<AST> Parser::ConstantDeclarationPart(){
    Consume(IDENTIFIER);
    Consume(EQUAL);
    Consume(NUMBER);
    Consume(SEMICOLON);
}

std::unique_ptr<AST> Parser::StatementPart(){
    Consume(KW_BEGIN);
    StatementSequence();
    Consume(KW_END);
}

std::unique_ptr<AST> Parser::IdentifierList(){
    Consume(IDENTIFIER);
    IdentifierListPrime();
}

std::unique_ptr<AST> Parser::IdentifierListPrime(){
    if(currentToken.type == COMMA){
        Consume(COMMA);
        Consume(IDENTIFIER);
        IdentifierListPrime();
    }
}

/************************/
/*      Procedures      */
/************************/

std::unique_ptr<AST> Parser::ProcedureDeclaration(){
    auto proto = ProcedureHeader();
    Consume(SEMICOLON);
    return ProcedureDeclarationPrime(std::move(proto));
}

std::unique_ptr<AST> Parser::ProcedureDeclarationPrime(std::unique_ptr<AST> dValue){
    if(currentToken.type == KW_FORWARD){
        Directive();
        Consume(SEMICOLON);
        return dValue;
    }
    else{
        auto body = Block();
        Consume(SEMICOLON);
        return llvm::make_unique<FunctionAST>(dValue, body);
    }
}


std::unique_ptr<AST> Parser::ProcedureHeader(){
    Consume(KW_PROCEDURE);
    auto identName = currentToken.identifierName;
    Consume(IDENTIFIER);
    return llvm::make_unique<PrototypeAST>(identName, ParameterList(), EOI);
}

std::vector<TypeNamePair> Parser::ParameterList(){
    if(currentToken.type == LEFTPAREN){
        Consume(LEFTPAREN);
        std::vector<TypeNamePair> params;
        params.push_back(Parameter());
        while(1){
            if(currentToken.type == COMMA){
                Consume(COMMA);
                params.push_back(Parameter());
            }
            else break;
        }
        Consume(RIGHTPAREN);
        return params;
    }
    return {};
}


TypeNamePair Parser::Parameter(){
    auto identName = currentToken.identifierName;
    Consume(IDENTIFIER);
    Consume(COLON);
    return TypeNamePair{Type(), identName};
}


/************************/
/*      Functions       */
/************************/

std::unique_ptr<AST> Parser::FunctionDeclaration(){
    auto proto = FunctionHeader();
    Consume(SEMICOLON);
    return FunctionDeclarationPrime(std::move(proto));
}

std::unique_ptr<AST> Parser::FunctionDeclarationPrime(std::unique_ptr<AST> dValue){
    if(currentToken.type == KW_FORWARD){
        Directive();
        Consume(SEMICOLON);
        //Was only prototype, so we can just return the prototype
        return dValue;
    }
    else{
        auto body = Block();
        Consume(SEMICOLON);
        return llvm::make_unique<FunctionAST>(dValue, body);
    }
}

std::unique_ptr<AST> Parser::FunctionHeader(){
    Consume(KW_FUNCTION);
    auto identName = currentToken.identifierName;
    Consume(IDENTIFIER);
    auto params = ParameterList();
    Consume(COLON);
    auto retType = Type();
    return llvm::make_unique<PrototypeAST>(identName, params, retType);
}

void Parser::Directive(){
    Consume(KW_FORWARD);
}


/************************/
/*      Statements      */
/************************/

std::unique_ptr<AST> Parser::StatementSequence(){
    std::vector<std::unique_ptr<AST>> statements;
    statements.push_back(std::move(Statement()));
    while(1){
        Consume(SEMICOLON);
        if( currentToken.type == KW_IF ||
            currentToken.type == KW_FOR ||
            currentToken.type == KW_WHILE ||
            currentToken.type == KW_BEGIN ||
            currentToken.type == IDENTIFIER){
            statements.push_back(std::move(Statement()));
        }
        else break;
    }
    return llvm::make_unique<StatementSequenceAST>(statements);
}


std::unique_ptr<AST> Parser::Statement(){
    switch(currentToken.type){
        case KW_IF:
            {
                return ConditionalStatement();
                break;
            }
        case KW_FOR:
        case KW_WHILE:
            {
                return RepeditiveStatement();
                break;
            }
        case KW_BEGIN:
            {
                return BlockStatment();
                break;
            }
        default:
            {
                return RegularStatement();
                break;
            }
    }
}

//Only here so I could add a switch or something later
std::unique_ptr<AST> Parser::ConditionalStatement(){
    return IfStatment();
}

std::unique_ptr<AST> Parser::IfStatment(){
    Consume(KW_IF);
    auto cond = Expression();
    Consume(KW_THEN);
    return llvm::make_unique<IfExpressionAST>(cond, Statement(), IfStatmentPrime());
}

std::unique_ptr<AST> Parser::IfStatmentPrime(){
    if(currentToken.type == KW_ELSE){
        Consume(KW_ELSE);
        return Statement();
    }
    return nullptr;
}

std::unique_ptr<AST> Parser::RepeditiveStatement(){
    switch(currentToken.type){
        case KW_FOR:
            {
                return ForStatement();
                break;
            }
        case KW_WHILE:
            {
                return WhileStatement();
                break;
            }
        default:
            {
                ConsumeError(KW_FOR);
                break;
            }
    }
    return nullptr;
}

std::unique_ptr<AST> Parser::WhileStatement(){
    Consume(KW_WHILE);
    auto cond = Expression();
    Consume(KW_DO);
    return llvm::make_unique<WhileExpressionAST>(cond, Statement());
}

std::unique_ptr<AST> Parser::ForStatement(){
    Consume(KW_FOR);
    auto identName = currentToken.identifierName;
    Consume(IDENTIFIER);
    Consume(ASSIGN);
    return ForStatementPrime(identName, Expression());
}

std::unique_ptr<AST> Parser::ForStatementPrime(std::string identifierName, std::unique_ptr<AST> start){
    LexicalTokenType direction = ERR;
    std::unique_ptr<AST> end;
    switch(currentToken.type){
        case KW_TO:
            {
                Consume(KW_TO);
                direction = KW_TO;
                end = Expression();
                break;
            }
        case KW_DOWNTO:
            {
                Consume(KW_DOWNTO);
                direction = KW_DOWNTO;
                end = Expression();
                break;
            }
        default:
            {
                ConsumeError(KW_TO);
                return;
            }
    }
    Consume(KW_DO);
    //TODO Step expression (if needed);
    return llvm::make_unique<ForExpressionAST>(identName, direction, start, end, nullptr, Statement());
}

std::unique_ptr<AST> Parser::BlockStatment(){
    Consume(KW_BEGIN);
    auto result = StatementSequence();
    Consume(KW_END);
    return std::move(result);
}

std::unique_ptr<AST> Parser::RegularStatement(){
    auto identName = currentToken.identifierName;
    Consume(IDENTIFIER);
    //array indexing
    if(currentToken.type == LEFTBRACKET){
        Consume(LEFTBRACKET);
        Expression();
        Consume(RIGHTBRACKET);
    }
    return RegularStatementPrime(identName);
}

std::unique_ptr<AST> Parser::RegularStatementPrime(std::string identifierName){
    switch(currentToken.type){
        case ASSIGN:
            {
                //TODO
                return AssignmentStatement();
                break;
            }
        case LEFTPAREN:
            {
                auto args = ProcdureStatement();
                return llvm::make_unique<CallExpessionsAst>(identifierName, std::move(args));
                break;
            }
        default:
            {
                ConsumeError(ASSIGN);
                break;
            }
    }
    return nullptr;
}

std::unique_ptr<AST> Parser::AssignmentStatement(){
    Consume(ASSIGN);
    Expression();
}

std::vector<std::unique_ptr<AST>> Parser::ProcdureStatement(){
    if(currentToken.type == LEFTPAREN){
        Consume(LEFTPAREN);
        auto result = UsageParameterList();
        Consume(RIGHTPAREN);
        return result;
    }
    return nullptr;
}

std::vector<std::unique_ptr<AST>> Parser::UsageParameterList(){
    std::vector<std::unique_ptr<AST>> args;
    while(1){
        auto arg = UsageParameter();
        args.push_back(std::move(arg));
        if(currentToken.type == COMMA){
            Consume(COMMA);
        }
        else{
            break;
        }
    }
    return std::move(args);
}


std::unique_ptr<AST> Parser::UsageParameter(){
    return Expression();
}


/************************/
/*      Expressions     */
/************************/

std::unique_ptr<AST> Parser::Expression(){
    return ExpressionPrime(BaseExpression());
}

std::unique_ptr<AST> Parser::ExpressionPrime(std::unique_ptr<AST> dValue){
    switch(currentToken.type){
        case EQUAL: case LESSTHAN: case LESSTHANEQ:
        case GREATERTHAN: case GREATERTHANEQ: case NOTEQUAL:
            {
                auto op = ComparisonOperator();
                ExpressionPrime(llvm::make_unique<BinaryOpAST>(op, dValue, BaseExpression()));
                break;
            }
        default:break;
    }
}

LexicalTokenType Parser::ComparisonOperator(){
    switch(currentToken.type){
        case EQUAL: Consume(EQUAL); return EQUAL;
        case LESSTHAN: Consume(LESSTHAN); return LESSTHAN;
        case LESSTHANEQ: Consume(LESSTHANEQ); return LESSTHANEQ;
        case GREATERTHAN: Consume(GREATERTHAN); return GREATERTHAN;
        case GREATERTHANEQ: Consume(GREATERTHANEQ); return GREATERTHANEQ;
        case NOTEQUAL: Consume(NOTEQUAL); return NOTEQUAL;
        default: ConsumeError(EQUAL); return ERR;
    }
}

std::unique_ptr<AST> Parser::BaseExpression(){
    if(currentToken.type == MINUS){
        Consume(MINUS);
    }
    return BaseExpressionPrime(Term());
}

std::unique_ptr<AST> Parser::BaseExpressionPrime(std::unique_ptr<AST> dValue){
    switch(currentToken.type){
        case PLUS: case MINUS: case OR:
            {
                auto op = PlusMinusOr(); 
                return BaseExpressionPrime(llvm::make_unique<BinaryOpAST>(op, dValue, Term()));
                break;
            }
        default:break;
    }
    return nullptr;
}

std::unique_ptr<AST> Parser::Term(){
   return TermPrime(Factor());
}

std::unique_ptr<AST> Parser::PlusMinusOr(){
    switch(currentToken.type){
        case PLUS: Consume(PLUS); return PLUS;
        case MINUS: Consume(MINUS); return MINUS;
        case OR: Consume(OR); return OR;
        default:return ERR;
    }
}

std::unique_ptr<AST> Parser::TermPrime(std::unique_ptr<AST> dValue){
    switch(currentToken.type){
        case TIMES: case DIVIDE: case AND: case MOD: case DIV:
            {
                auto op = MultDivAnd();
                TermPrime(llvm::make_unique<BinaryOpAST>(op, dValue, Factor()));
                break;
            }
        default: break;
    }
    return nullptr;
}

LexicalTokenType Parser::MultDivAnd(){
    switch(currentToken.type){
        case TIMES: Consume(TIMES); return TIMES;
        case DIVIDE: Consume(DIVIDE); return DIVIDE;
        case AND: Consume(AND); return AND;
        case MOD: Consume(MOD); return MOD;
        case DIV: Consume(DIV); return DIV;
        default:return ERR;
    }
}

std::unique_ptr<AST> Parser::Factor(){
    switch(currentToken.type){
        case IDENTIFIER:
            {
                auto identName = currentToken.identifierName;
                Consume(IDENTIFIER);
                if(currentToken.type == LEFTPAREN){
                    auto args = ProcdureStatement();
                    return llvm::make_unique<CallExpessionsAst>(identName, args);
                }
                //TODO
                //Array index
                else if(currentToken.type == LEFTBRACKET){
                    Consume(LEFTBRACKET);
                    Expression();
                    Consume(RIGHTBRACKET);
                }
                //simple variable reference
                else{
                    return llvm::make_unique<VariableIdentifierAST>(identName);
                }
                break;
            }
        case NUMBER:
            {
                auto result = llvm::make_unique<NumberAST>(currentToken.storedNumber);
                Consume(NUMBER);
                return std::move(result);
            }
        case LEFTPAREN:
            {
                Consume(LEFTPAREN);
                auto result = Expression();
                Consume(RIGHTPAREN);
                return result;
            }
        default:
            {
                ConsumeError(IDENTIFIER);
                break;
            }
    }
    return nullptr;
}




/************************/
/*         MISC         */
/************************/

//TODO 
std::unique_ptr<AST> Parser::Type(){
    switch(currentToken.type){
        case KW_INTEGER:
            {
                Consume(KW_INTEGER);
                break;
            }
        case KW_ARRAY:
            {
                Consume(KW_ARRAY);
                Consume(LEFTBRACKET);
                Expression();
                Consume(DOTDOT);
                Expression();
                Consume(RIGHTBRACKET);
                Consume(KW_OF);
                Type();
                break;
            }
        default:
            {
                ConsumeError(KW_INTEGER);
                break;
            }
    }
    return nullptr;
}




/*
void Parser::F(){
    switch(currentToken.type){
        case NUMBER:
            {
                printf("Number %d\n", currentToken.storedNumber);
                Consume(currentToken.type);
                break;
            }
        case LEFTPAREN:
            {
                Consume(currentToken.type);
                E();
                Consume(RIGHTPAREN);
                break;
            }
        default:
            {
                ConsumeError(NUMBER);
            }
    }
}

*/

