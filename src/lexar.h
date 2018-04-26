#ifndef LEXAR_H
#define LEXAR_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>


#define MAX_LINE_LENGTH 257
#define MAX_IDENTIFIER_LENGTH 32

enum InputCharType {
	LETTER, NUMB, END, WHITE_SPACE, NO_TYPE
};
enum LexicalTokenType{
	IDENTIFIER, NUMBER, PLUS, MINUS, TIMES, DIVIDE, AND, OR, MOD, DIV,
	EQUAL, NOTEQUAL, LESSTHAN, GREATERTHAN, LESSTHANEQ, GREATERTHANEQ,
	LEFTPAREN, RIGHTPAREN, LEFTBRACKET, RIGHTBRACKET,
	ASSIGN, COMMA, COLON, SEMICOLON, DOTDOT, DOT, KW_VAR, KW_CONST,
	KW_IF, KW_THEN, KW_ELSE, KW_BEGIN, KW_END,
	KW_WHILE, KW_DO, KW_READ, KW_WRITE, 
    KW_FOR, KW_TO, KW_DOWNTO, 
    KW_PROGRAM, KW_PROCEDURE, KW_FUNCTION, KW_FORWARD, KW_ARRAY, KW_INTEGER, KW_OF, 
	EOI, ERR
};
extern const char *lexicalTokenNames[49];

struct LexicalToken{
	LexicalTokenType type;
	int storedNumber;
	std::string identifierName;
};

struct InputToken{
	InputCharType type;
	char value;
};

struct InputElement{
    std::istream* inputFile;
    std::string inputText;
};

class Lexar{
	public:
		Lexar();
		~Lexar();
        std::string fileName;
		bool Init(const char* fileName);
        bool Init(std::string);
		LexicalToken NextToken();
		int lineNumber;
		int columnNumber;
	private:
        InputElement inputElement;
		InputToken currentInput;

		char GetNextChar();
		InputToken ReadInput();
		LexicalTokenType GetKeyWord(std::string id);
		LexicalToken HandleIdentKeyword();
		LexicalToken HandleNumber();
		LexicalToken HandleSpecialChars();
		void HandleComments();

		void Error(std::string message);
};

#endif
