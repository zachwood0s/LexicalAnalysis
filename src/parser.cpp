#include <stdlib.h>
#include "parser.h"
#include "lexar.h"


Parser::Parser(Lexar* lexar){
    this->lexar = lexar;
}

void Parser::ConsumeError(LexicalTokenType type){
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
        //E();
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
}

void Parser::ProgramHeader(){
    Consume(KW_PROGRAM);
    Consume(IDENTIFIER);
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
                break;
            }
        case KW_CONST:
            {
                ConstantDeclaration();
                break;
            }
        case KW_PROCEDURE:
            {
                ProcedureDeclaration();
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
    if(currentToken.type == SEMICOLON){
        Consume(SEMICOLON);
        VariableDeclarationPart();
        VariableDeclarationPrime();
    }
}

void Parser::VariableDeclarationPart(){
    IdentifierList();
    Consume(COLON);
    Type();
}

void Parser::ConstantDeclaration(){
    Consume(KW_CONST);
    ConstantDeclarationPart();
    ConstantDeclarationPrime();
}

void Parser::ConstantDeclarationPrime(){
    if(currentToken.type == SEMICOLON){
        Consume(SEMICOLON);
        ConstantDeclarationPart();
        ConstantDeclarationPrime();
    } 
}

void Parser::ConstantDeclarationPart(){
    Consume(IDENTIFIER);
    Consume(ASSIGN);
    Consume(NUMBER);
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
    Consume(KW_PROCEDURE);
    Consume(IDENTIFIER);
    ParameterList();
    ReturnType();
    Consume(SEMICOLON);
}

void Parser::ParameterList(){
    Parameter();
    ParameterListPrime();
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

void Parser::ReturnType(){
    if(currentToken.type == COLON){
        Consume(COLON);
        Type();
    }
}

/************************/
/*      Statements      */
/************************/

void Parser::StatementSequence(){
    Statement();
    StatementSequencePrime();
}

void Parser::StatementSequencePrime(){
    if(currentToken.type == SEMICOLON){
        Consume(SEMICOLON);
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
}

void Parser::BlockStatment(){

}

void Parser::RegularStatement(){

}

void Parser::RegularStatementPrime(){

}

void Parser::AssignmentStatement(){

}

void Parser::ProcdureStatement(){

}

void Parser::UsageParameterList(){

}

void Parser::UsageParameterListPrime(){

}

void Parser::UsageParameter(){

}


/************************/
/*      Expressions     */
/************************/

void Parser::Expression(){

}

void Parser::ExpressionPrime(){

}

void Parser::ComparisonOperator(){

}

void Parser::BaseExpression(){

}

void Parser::BaseExpressionPrime(){

}

void Parser::Term(){

}

void Parser::PlusMinusOr(){

}

void Parser::TermPrime(){

}

void Parser::MultDivAnd(){

}

void Parser::Factor(){

}


/************************/
/*         MISC         */
/************************/

void Parser::Type(){

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

