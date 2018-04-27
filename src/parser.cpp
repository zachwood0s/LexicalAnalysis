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

void Parser::Program(){
    ProgramHeader();
    Block();
    Consume(DOT);
}

void Parser::ProgramHeader(){
    Consume(KW_PROGRAM);
    Consume(IDENTIFIER);
    Consume(SEMICOLON);
}

void Parser::Block(){
    DeclarationPart();
    StatementPart();
}

void Parser::DeclarationPart(){
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

void Parser::VariableDeclaration(){
    Consume(KW_VAR);
    VariableDeclarationPart();
    VariableDeclarationPrime();
}

void Parser::VariableDeclarationPrime(){
    if(currentToken.type == IDENTIFIER){
        VariableDeclarationPart();
        VariableDeclarationPrime();
    }
}

void Parser::VariableDeclarationPart(){
    IdentifierList();
    Consume(COLON);
    Type();
    Consume(SEMICOLON);
}

void Parser::ConstantDeclaration(){
    Consume(KW_CONST);
    ConstantDeclarationPart();
    ConstantDeclarationPrime();
}

void Parser::ConstantDeclarationPrime(){
    if(currentToken.type == IDENTIFIER){
        ConstantDeclarationPart();
        ConstantDeclarationPrime();
    } 
}

void Parser::ConstantDeclarationPart(){
    Consume(IDENTIFIER);
    Consume(EQUAL);
    Consume(NUMBER);
    Consume(SEMICOLON);
}

void Parser::StatementPart(){
    Consume(KW_BEGIN);
    StatementSequence();
    Consume(KW_END);
}

void Parser::IdentifierList(){
    Consume(IDENTIFIER);
    IdentifierListPrime();
}

void Parser::IdentifierListPrime(){
    if(currentToken.type == COMMA){
        Consume(COMMA);
        Consume(IDENTIFIER);
        IdentifierListPrime();
    }
}

/************************/
/*      Procedures      */
/************************/

void Parser::ProcedureDeclaration(){
    ProcedureHeader();
    Consume(SEMICOLON);
    ProcedureDeclarationPrime();
}

void Parser::ProcedureDeclarationPrime(){
    if(currentToken.type == KW_FORWARD){
        Directive();
        Consume(SEMICOLON);
    }
    else{
        Block();
        Consume(SEMICOLON);
    }
}


void Parser::ProcedureHeader(){
    Consume(KW_PROCEDURE);
    Consume(IDENTIFIER);
    ParameterList();
}

void Parser::ParameterList(){
    if(currentToken.type == LEFTPAREN){
        Consume(LEFTPAREN);
        Parameter();
        ParameterListPrime();
        Consume(RIGHTPAREN);
    }
}

void Parser::ParameterListPrime(){
    if(currentToken.type == COMMA){
        Consume(COMMA);
        Parameter();
        ParameterListPrime();
    }
}

void Parser::Parameter(){
    Consume(IDENTIFIER);
    Consume(COLON);
    Type();
}


/************************/
/*      Functions       */
/************************/

void Parser::FunctionDeclaration(){
    FunctionHeader();
    Consume(SEMICOLON);
    FunctionDeclarationPrime();
}

void Parser::FunctionDeclarationPrime(){
    if(currentToken.type == KW_FORWARD){
        Directive();
        Consume(SEMICOLON);
    }
    else{
        Block();
        Consume(SEMICOLON);
    }
}

void Parser::FunctionHeader(){
    Consume(KW_FUNCTION);
    Consume(IDENTIFIER);
    ParameterList();
    Consume(COLON);
    Type();
}

void Parser::Directive(){
    Consume(KW_FORWARD);
}


/************************/
/*      Statements      */
/************************/

void Parser::StatementSequence(){
    Statement();
    StatementSequencePrime();
}

void Parser::StatementSequencePrime(){
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

void Parser::Statement(){
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
void Parser::ConditionalStatement(){
    IfStatment();
}

void Parser::IfStatment(){
    Consume(KW_IF);
    Expression();
    Consume(KW_THEN);
    Statement();
    IfStatmentPrime();
}

void Parser::IfStatmentPrime(){
    if(currentToken.type == KW_ELSE){
        Consume(KW_ELSE);
        Statement();
    }
}

void Parser::RepeditiveStatement(){
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

void Parser::WhileStatement(){
    Consume(KW_WHILE);
    Expression();
    Consume(KW_DO);
    Statement();
}

void Parser::ForStatement(){
    Consume(KW_FOR);
    Consume(IDENTIFIER);
    Consume(ASSIGN);
    Expression();
    ForStatementPrime();
}

void Parser::ForStatementPrime(){
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

void Parser::BlockStatment(){
    Consume(KW_BEGIN);
    StatementSequence();
    Consume(KW_END);
}

void Parser::RegularStatement(){
    Consume(IDENTIFIER);
    if(currentToken.type == LEFTBRACKET){
        Consume(LEFTBRACKET);
        Expression();
        Consume(RIGHTBRACKET);
    }
    RegularStatementPrime();
}

void Parser::RegularStatementPrime(){
    switch(currentToken.type){
        case ASSIGN:
            {
                AssignmentStatement();
                break;
            }
        case LEFTPAREN:
            {
                ProcdureStatement();
                break;
            }
        default:
            {
                ConsumeError(ASSIGN);
                break;
            }
    }
}

void Parser::AssignmentStatement(){
    Consume(ASSIGN);
    Expression();
}

void Parser::ProcdureStatement(){
    if(currentToken.type == LEFTPAREN){
        Consume(LEFTPAREN);
        UsageParameterList();
        Consume(RIGHTPAREN);
    }
}

void Parser::UsageParameterList(){
    UsageParameter();
    UsageParameterListPrime();
}

void Parser::UsageParameterListPrime(){
    if(currentToken.type == COMMA){
        Consume(COMMA);
        UsageParameter();
        UsageParameterListPrime();
    }
}

void Parser::UsageParameter(){
    Expression();
}


/************************/
/*      Expressions     */
/************************/

void Parser::Expression(){
    BaseExpression();
    ExpressionPrime();
}

void Parser::ExpressionPrime(){
    switch(currentToken.type){
        case EQUAL: case LESSTHAN: case LESSTHANEQ:
        case GREATERTHAN: case GREATERTHANEQ: case NOTEQUAL:
            {
                ComparisonOperator();
                BaseExpression();
                ExpressionPrime();
                break;
            }
        default:break;
    }
}

void Parser::ComparisonOperator(){
    switch(currentToken.type){
        case EQUAL: Consume(EQUAL); break;
        case LESSTHAN: Consume(LESSTHAN); break;
        case LESSTHANEQ: Consume(LESSTHANEQ); break;
        case GREATERTHAN: Consume(GREATERTHAN); break;
        case GREATERTHANEQ: Consume(GREATERTHANEQ); break;
        case NOTEQUAL: Consume(NOTEQUAL); break;
        default: ConsumeError(EQUAL); break;
    }
}

void Parser::BaseExpression(){
    if(currentToken.type == MINUS){
        Consume(MINUS);
    }
    Term();
    BaseExpressionPrime();
}

void Parser::BaseExpressionPrime(){
    switch(currentToken.type){
        case PLUS: case MINUS: case OR:
            {
                PlusMinusOr(); 
                Term();
                BaseExpressionPrime();
                break;
            }
        default:break;
    }
}

void Parser::Term(){
   Factor();
   TermPrime();
}

void Parser::PlusMinusOr(){
    switch(currentToken.type){
        case PLUS: Consume(PLUS); break;
        case MINUS: Consume(MINUS); break;
        case OR: Consume(OR); break;
        default:break;
    }
}

void Parser::TermPrime(){
    switch(currentToken.type){
        case TIMES: case DIVIDE: case AND: case MOD: case DIV:
            {
                MultDivAnd();
                Factor();
                TermPrime();
                break;
            }
        default:break;
    }
}

void Parser::MultDivAnd(){
    switch(currentToken.type){
        case TIMES: Consume(TIMES); break;
        case DIVIDE: Consume(DIVIDE); break;
        case AND: Consume(AND); break;
        case MOD: Consume(MOD); break;
        case DIV: Consume(DIV); break;
        default:break;
    }
}

void Parser::Factor(){
    switch(currentToken.type){
        case IDENTIFIER:
            {
                Consume(IDENTIFIER);
                if(currentToken.type == LEFTPAREN){
                    ProcdureStatement();
                }
                else if(currentToken.type == LEFTBRACKET){
                    Consume(LEFTBRACKET);
                    Expression();
                    Consume(RIGHTBRACKET);
                }
                break;
            }
        case NUMBER:
            {
                Consume(NUMBER);
                break;
            }
        case LEFTPAREN:
            {
                Consume(LEFTPAREN);
                Expression();
                Consume(RIGHTPAREN);
                break;
            }
        default:
            {
                ConsumeError(IDENTIFIER);
                break;
            }
    }
}




/************************/
/*         MISC         */
/************************/

void Parser::Type(){
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

