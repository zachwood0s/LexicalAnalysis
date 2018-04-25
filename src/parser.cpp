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
        VariableDeclarationPart();
        VariableDeclarationPrime();
    }
}

void Parser::VariableDeclarationPart(){
    Iden
}

void Parser::ConstantDeclaration(){

}

void Parser::ConstantDeclarationPrime(){

}

void Parser::ConstantDeclarationPart(){

}

void Parser::StatementPart(){

}

/************************/
/*      Procedures      */
/************************/

void Parser::ProcedureDeclaration(){

}

void Parser::ParameterList(){

}

void Parser::ParameterListPrime(){

}

void Parser::Parameter(){

}

void Parser::ReturnType(){

}

/************************/
/*      Statements      */
/************************/

void Parser::StatementSequence(){

}

void Parser::StatementSequencePrime(){

}

void Parser::Statement(){

}

void Parser::ConditionalStatement(){

}

void Parser::IfStatment(){

}

void Parser::RepeditiveStatement(){

}

void Parser::WhileStatement(){

}

void Parser::ForStatement(){

}

void Parser::ForStatementPrime(){

}

void Parser::BlockStatment(){

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

void Parser::TermPrime(){

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

