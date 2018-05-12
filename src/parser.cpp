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
    ProcedureHeader();
    Consume(SEMICOLON);
    ProcedureDeclarationPrime();
}

std::unique_ptr<AST> Parser::ProcedureDeclarationPrime(){
    if(currentToken.type == KW_FORWARD){
        Directive();
        Consume(SEMICOLON);
    }
    else{
        Block();
        Consume(SEMICOLON);
    }
}


std::unique_ptr<AST> Parser::ProcedureHeader(){
    Consume(KW_PROCEDURE);
    Consume(IDENTIFIER);
    ParameterList();
}

std::unique_ptr<AST> Parser::ParameterList(){
    if(currentToken.type == LEFTPAREN){
        Consume(LEFTPAREN);
        Parameter();
        ParameterListPrime();
        Consume(RIGHTPAREN);
    }
}

std::unique_ptr<AST> Parser::ParameterListPrime(){
    if(currentToken.type == COMMA){
        Consume(COMMA);
        Parameter();
        ParameterListPrime();
    }
}

std::unique_ptr<AST> Parser::Parameter(){
    Consume(IDENTIFIER);
    Consume(COLON);
    Type();
}


/************************/
/*      Functions       */
/************************/

std::unique_ptr<AST> Parser::FunctionDeclaration(){
    FunctionHeader();
    Consume(SEMICOLON);
    FunctionDeclarationPrime();
}

std::unique_ptr<AST> Parser::FunctionDeclarationPrime(){
    if(currentToken.type == KW_FORWARD){
        Directive();
        Consume(SEMICOLON);
    }
    else{
        Block();
        Consume(SEMICOLON);
    }
}

std::unique_ptr<AST> Parser::FunctionHeader(){
    Consume(KW_FUNCTION);
    Consume(IDENTIFIER);
    ParameterList();
    Consume(COLON);
    Type();
}

std::unique_ptr<AST> Parser::Directive(){
    Consume(KW_FORWARD);
}


/************************/
/*      Statements      */
/************************/

std::unique_ptr<AST> Parser::StatementSequence(){
    Statement();
    StatementSequencePrime();
}

std::unique_ptr<AST> Parser::StatementSequencePrime(){
    Consume(SEMICOLON);
    if( currentToken.type == KW_IF ||
        currentToken.type == KW_FOR ||
        currentToken.type == KW_WHILE ||
        currentToken.type == KW_BEGIN ||
        currentToken.type == IDENTIFIER){
        Statement();
        StatementSequencePrime();
    }
}

std::unique_ptr<AST> Parser::Statement(){
    switch(currentToken.type){
        case KW_IF:
            {
                ConditionalStatement();
                break;
            }
        case KW_FOR:
        case KW_WHILE:
            {
                RepeditiveStatement();
                break;
            }
        case KW_BEGIN:
            {
                BlockStatment();
                break;
            }
        default:
            {
                RegularStatement();
                break;
            }
    }
}

//Only here so I could add a switch or something later
std::unique_ptr<AST> Parser::ConditionalStatement(){
    IfStatment();
}

std::unique_ptr<AST> Parser::IfStatment(){
    Consume(KW_IF);
    Expression();
    Consume(KW_THEN);
    Statement();
    IfStatmentPrime();
}

std::unique_ptr<AST> Parser::IfStatmentPrime(){
    if(currentToken.type == KW_ELSE){
        Consume(KW_ELSE);
        Statement();
    }
}

std::unique_ptr<AST> Parser::RepeditiveStatement(){
    switch(currentToken.type){
        case KW_FOR:
            {
                ForStatement();
                break;
            }
        case KW_WHILE:
            {
                WhileStatement();
                break;
            }
        default:
            {
                ConsumeError(KW_FOR);
                break;
            }
    }
}

std::unique_ptr<AST> Parser::WhileStatement(){
    Consume(KW_WHILE);
    Expression();
    Consume(KW_DO);
    Statement();
}

std::unique_ptr<AST> Parser::ForStatement(){
    Consume(KW_FOR);
    Consume(IDENTIFIER);
    Consume(ASSIGN);
    Expression();
    ForStatementPrime();
}

std::unique_ptr<AST> Parser::ForStatementPrime(){
    switch(currentToken.type){
        case KW_TO:
            {
                Consume(KW_TO);
                Expression();
                break;
            }
        case KW_DOWNTO:
            {
                Consume(KW_DOWNTO);
                Expression();
                break;
            }
        default:
            {
                ConsumeError(KW_TO);
                return;
            }
    }
    Consume(KW_DO);
    Statement();
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

