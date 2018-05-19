#include <stdlib.h>
#include "parser.h"
#include "lexar.h"

static std::vector<std::string> functionNames = {"exit", "writeln", "readln"};

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
        this->tree = Program();
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
    auto header = ProgramHeader();
    auto declarations = DeclarationPart();
    Consume(KW_BEGIN);
    auto statements = StatementSequence();
    Consume(KW_END);
    Consume(DOT);
    return llvm::make_unique<ProgramAST>(header, std::move(declarations), std::move(statements));
}

std::string Parser::ProgramHeader(){
    Consume(KW_PROGRAM);
    auto programName = currentToken.identifierName;
    Consume(IDENTIFIER);
    Consume(SEMICOLON);
    return programName;
}

std::unique_ptr<AST> Parser::Block(){
    return StatementPart(DeclarationPart());
}

std::vector<std::unique_ptr<AST>> Parser::DeclarationPart(){
    bool moreDeclarations = true;
    std::vector<std::unique_ptr<AST>> declarations;

    while(moreDeclarations){
        switch(currentToken.type){
            case KW_VAR:
                {
                    declarations.push_back(VariableDeclaration());
                    break;
                }
            case KW_CONST:
                {
                    declarations.push_back(ConstantDeclaration());
                    break;
                }
            case KW_PROCEDURE:
                {
                    declarations.push_back(ProcedureDeclaration());
                    break;
                }
            case KW_FUNCTION:
                {
                    declarations.push_back(FunctionDeclaration());
                    break;
                }
            default: moreDeclarations = false; break;
        }
    }
    return declarations;
}

std::unique_ptr<AST> Parser::VariableDeclaration(){
    Consume(KW_VAR);
    std::vector<std::unique_ptr<AST>> declarations;
    do{
        declarations.push_back(VariableDeclarationPart());
    }
    while(currentToken.type == IDENTIFIER);
    return llvm::make_unique<VariableDeclarationsAST>(std::move(declarations));
}

std::unique_ptr<AST> Parser::VariableDeclarationPart(){
    auto idents = IdentifierList();
    Consume(COLON);
    auto type = Type();
    Consume(SEMICOLON);
    return llvm::make_unique<VariableDeclarationsOfTypeAST>(std::move(idents), type);
}

std::unique_ptr<AST> Parser::ConstantDeclaration(){
    Consume(KW_CONST);
    std::vector<ValueNamePair> constants;
    do{
       constants.push_back(ConstantDeclarationPart()); 
    }
    while(currentToken.type == IDENTIFIER);
    return llvm::make_unique<ConstantDeclarationsAST>(std::move(constants));
}


ValueNamePair Parser::ConstantDeclarationPart(){
    auto identName = currentToken.identifierName;
    Consume(IDENTIFIER);
    Consume(EQUAL);
    auto value = currentToken.storedNumber;
    Consume(NUMBER);
    Consume(SEMICOLON);
    return {value, identName};
}

std::unique_ptr<AST> Parser::StatementPart(std::vector<std::unique_ptr<AST>> dValue){
    Consume(KW_BEGIN);
    auto result = StatementSequence();
    Consume(KW_END);
    return llvm::make_unique<MainBlockAST>(std::move(dValue), std::move(result));
}

std::vector<std::unique_ptr<VariableIdentifierAST>> Parser::IdentifierList(){
    std::vector<std::unique_ptr<VariableIdentifierAST>> identifiers;
    auto identName = currentToken.identifierName;
    Consume(IDENTIFIER);
    auto identAST = llvm::make_unique<VariableIdentifierAST>(identName);
    identifiers.push_back(std::move(identAST));

    while(currentToken.type == COMMA){
        Consume(COMMA);
        auto identName = currentToken.identifierName;
        Consume(IDENTIFIER);
        auto identAST = llvm::make_unique<VariableIdentifierAST>(identName);
        identifiers.push_back(std::move(identAST));
    }
    return identifiers;
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
        return llvm::make_unique<FunctionAST>(std::move(dValue), std::move(body));
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
        return llvm::make_unique<FunctionAST>(std::move(dValue), std::move(body));
    }
}

std::unique_ptr<AST> Parser::FunctionHeader(){
    Consume(KW_FUNCTION);
    auto identName = currentToken.identifierName;
    Consume(IDENTIFIER);
    auto params = ParameterList();
    Consume(COLON);
    auto retType = Type();
    return llvm::make_unique<PrototypeAST>(identName, std::move(params), retType);
}

void Parser::Directive(){
    Consume(KW_FORWARD);
}


/************************/
/*      Statements      */
/************************/

std::unique_ptr<AST> Parser::StatementSequence(){
    std::vector<std::unique_ptr<AST>> statements;
    statements.push_back(Statement());
    while(1){
        Consume(SEMICOLON);
        if( currentToken.type == KW_IF ||
            currentToken.type == KW_FOR ||
            currentToken.type == KW_WHILE ||
            currentToken.type == KW_BEGIN ||
            currentToken.type == IDENTIFIER ||
            currentToken.type == KW_EXIT ||
            currentToken.type == KW_BREAK){
            statements.push_back(Statement());
        }
        else break;
    }
    return llvm::make_unique<StatementSequenceAST>(std::move(statements));
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
    auto thenPart = Statement();
    auto elsePart = IfStatmentPrime();
    return llvm::make_unique<IfExpressionAST>(std::move(cond), std::move(thenPart), std::move(elsePart));
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
    return llvm::make_unique<WhileExpressionAST>(std::move(cond), Statement());
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
                return nullptr;
            }
    }
    Consume(KW_DO);
    //TODO Step expression (if needed);
    return llvm::make_unique<ForExpressionAST>(identifierName, direction, std::move(start), std::move(end), nullptr, Statement());
}

std::unique_ptr<AST> Parser::BlockStatment(){
    Consume(KW_BEGIN);
    auto result = StatementSequence();
    Consume(KW_END);
    return result;
}

std::unique_ptr<AST> Parser::RegularStatement(){
    if(currentToken.type == KW_EXIT ||
       currentToken.type == KW_BREAK){
        auto res = llvm::make_unique<ExitBreakStatementAST>(currentToken.type);
        Consume(currentToken.type);
        return res; 
    }
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
                auto var = llvm::make_unique<VariableIdentifierAST>(identifierName);
                return llvm::make_unique<BinaryOpAST>(ASSIGN, std::move(var), AssignmentStatement());
            }
        case LEFTPAREN:
            {
                auto args = ProcdureStatement();
                return llvm::make_unique<CallExpessionsAst>(identifierName, std::move(args));
            }
        default:
            {
                if(std::find(functionNames.begin(), functionNames.end(),identifierName) != functionNames.end()){
                    //Is function with no args
                    return llvm::make_unique<CallExpessionsAst>(identifierName);
                }
                
                else{
                    ConsumeError(ASSIGN);
                }
            }
    }
    return nullptr;
}

std::unique_ptr<AST> Parser::AssignmentStatement(){
    Consume(ASSIGN);
    return Expression();
}

std::vector<std::unique_ptr<AST>> Parser::ProcdureStatement(){
    if(currentToken.type == LEFTPAREN){
        Consume(LEFTPAREN);
        auto result = UsageParameterList();
        Consume(RIGHTPAREN);
        return result;
    }
    return {};
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
    return args;
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
                auto res = llvm::make_unique<ComparisonOpAST>(op, std::move(dValue), BaseExpression());
                return ExpressionPrime(std::move(res));
                break;
            }
        default:break;
    }
    return dValue;
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
        return llvm::make_unique<UnaryOpAST>(MINUS, BaseExpressionPrime(Term()));
    }
    return BaseExpressionPrime(Term());
}

std::unique_ptr<AST> Parser::BaseExpressionPrime(std::unique_ptr<AST> dValue){
    switch(currentToken.type){
        case PLUS: case MINUS: case OR:
            {
                auto op = PlusMinusOr(); 
                auto res = llvm::make_unique<BinaryOpAST>(op, std::move(dValue), Term());
                return BaseExpressionPrime(std::move(res));
                break;
            }
        default:break;
    }
    return dValue;
}

std::unique_ptr<AST> Parser::Term(){
   return TermPrime(Factor());
}

LexicalTokenType Parser::PlusMinusOr(){
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
                return TermPrime(llvm::make_unique<BinaryOpAST>(op, std::move(dValue), Factor()));
                break;
            }
        default: break;
    }
    return dValue;
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
                    return llvm::make_unique<CallExpessionsAst>(identName, std::move(args));
                }
                //TODO
                //Array index
                else if(currentToken.type == LEFTBRACKET){
                    Consume(LEFTBRACKET);
                    Expression();
                    Consume(RIGHTBRACKET);
                    //TODO not actually doing anything with the arrays
                    return llvm::make_unique<VariableIdentifierAST>(identName);
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
LexicalTokenType Parser::Type(){
    switch(currentToken.type){
        case KW_INTEGER:
            {
                Consume(KW_INTEGER);
                return KW_INTEGER;
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
                return KW_ARRAY;
                break;
            }
        default:
            {
                ConsumeError(KW_INTEGER);
                break;
            }
    }
    return ERR;
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

