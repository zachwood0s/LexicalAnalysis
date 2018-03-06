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
	IDENTIFIER, NUMBER, PLUS, MINUS, TIMES, DIVIDE,
	EQUAL, NOTEQUAL, LESSTHAN, GREATERTHAN, LESSTHANEQ, GREATERTHANEQ,
	LEFTPAREN, RIGHTPAREN,
	ASSIGN, COMMA, SEMICOLON, KW_VAR, KW_CONST,
	KW_IF, KW_THEN, KW_ELSE, KW_BEGIN, KW_END,
	KW_WHILE, KW_DO, KW_WRITE, KW_READ,
	EOI, ERR
};
extern const char *lexicalTokenNames[30];

struct LexicalToken{
	LexicalTokenType type;
	int storedNumber;
	std::string identifierName;
};

struct InputToken{
	InputCharType type;
	char value;
};

class Lexar{
	public:
		Lexar();
		~Lexar();
		bool Init(char* fileName);
		LexicalToken NextToken();
		int lineNumber;
		int columnNumber;
	private:
		std::istream* inputFile;
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
