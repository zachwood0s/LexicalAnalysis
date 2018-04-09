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
        E();
        Consume(EOI);
        return true;
    } catch(const char * msg){
        printf("%s\nExiting\n", msg);
    }
    return false;
}

void Parser::E(){
    switch(currentToken.type){
        case NUMBER:
        case LEFTPAREN:
            {
                T();
                EPrime();
                break;
            }
        default:
            {
                ConsumeError(NUMBER);
                break;
            }
    } 
}

void Parser::EPrime(){
    switch(currentToken.type){
        case PLUS:
        case MINUS:
            {
                Consume(currentToken.type);
                T();
                EPrime();
                break;
            }
        default:
            break;
    }
}

void Parser::T(){
    switch(currentToken.type){
        case NUMBER:
        case LEFTPAREN:
            {
                F();
                TPrime();
                break;
            }
        default:
            {
                ConsumeError(NUMBER);
                break;
            }
    }
}

void Parser::TPrime(){
    switch(currentToken.type){
        case TIMES:
        case DIVIDE:
            {
                Consume(currentToken.type);
                F();
                TPrime();
                break;
            }
        default:
            break;
    }
}

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


