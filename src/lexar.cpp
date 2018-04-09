#include <stdio.h>
#include <sstream>
#include <string.h>
#include "lexar.h"

using namespace std;

const char *lexicalTokenNames[30] = {
	"IDENTIFIER", "NUMBER", "PLUS", "MINUS", "TIMES", "DIVIDE",
	"EQUAL", "NOTEQUAL", "LESSTHAN", "GREATERTHAN", "LESSTHANEQ", "GREATERTHANEQ",
	"LEFTPAREN", "RIGHTPAREN",
	"ASSIGN", "COMMA", "SEMICOLON", "kwVAR", "kwCONST", 
	"kwIF","kwTHEN","kwELSE", "kwBEGIN", "kwEND",  
	"kwWHILE", "kwDO", "kwWRITE", "kwREAD",
	"EOI", "ERR"
};
const struct {const char* word; LexicalTokenType symb;} keyWordTable[] ={
	{"var", KW_VAR},
	{"const", KW_CONST},
	{"begin", KW_BEGIN},
	{"end", KW_END},
	{"if", KW_IF},
	{"then", KW_THEN},
	{"else", KW_ELSE},
	{"while", KW_WHILE},
	{"do", KW_DO},
	{"write", KW_WRITE},
	{"read", KW_READ},
	{NULL, (LexicalTokenType) 0}
};

const struct {const char specialCharacter; LexicalTokenType token;} specialCharacterTable[]={
	{';', SEMICOLON},
	{',', COMMA},
	{'+', PLUS},
	{'-', MINUS},
	{'*', TIMES},
	{'/', DIVIDE},
	{'(', LEFTPAREN},
	{')', RIGHTPAREN},
	{0, (LexicalTokenType) 0}
};

const char BlockCommentOpen = '{';
const char BlockCommentClose = '}';	

Lexar::Lexar(){
    inputElement.inputFile = NULL;
}
Lexar::~Lexar(){
	if(inputElement.inputText.size() == 0 && inputElement.inputFile != &std::cin){
		delete inputElement.inputFile;
	}
	inputElement.inputFile = NULL;
}

void Lexar::Error(string message){
	printf("ERROR ln: %d, col: %d; %s\n", lineNumber,columnNumber, message.c_str());
}

bool Lexar::Init(char* fileName){
	lineNumber = 1;
	if(!fileName){
		inputElement.inputFile = &std::cin;
	}
	else{
		inputElement.inputFile = new ifstream(fileName);
		if(!inputElement.inputFile){
			printf("Error opening %s \n",fileName);
			return false;
		}
	}
	currentInput = ReadInput();
	return true;
}

bool Lexar::Init(string input){
    inputElement.inputText = input;
	currentInput = ReadInput();
	columnNumber = 0;
    return true;
}

LexicalTokenType Lexar::GetKeyWord(string id){
	int i = 0;
	while(keyWordTable[i].word){
		if(id.compare(keyWordTable[i].word) == 0){
			return keyWordTable[i].symb;
		}
		else i++;
	}
	return IDENTIFIER;
}

char Lexar::GetNextChar(){
    char next;
    if(inputElement.inputFile != NULL){
        next = inputElement.inputFile->get();
    }
    else{
        if(inputElement.inputText.size() > 0){
            next = inputElement.inputText.at(0);
            inputElement.inputText.erase(0, 1);
        }
        else{
            next = EOF;
        }
    }
    columnNumber++;
    if(next == '\n') {
        lineNumber++;
        columnNumber = 0;
    }
    return next;
}

InputToken Lexar::ReadInput() {
	int character = GetNextChar();
	InputToken input;
	input.value = character;

	if ((character>='A' && character<='Z') || (character>='a' && character<='z'))
	  input.type = LETTER;	
	else if (character>='0' && character<='9')
	  input.type = NUMB;
	else if (character == EOF)
	  input.type = END;
	else if (character <= ' ')
	  input.type = WHITE_SPACE;
	else
	  input.type = NO_TYPE;
	return input;
}

LexicalToken Lexar::NextToken(){
	//Consume all white space
	while(currentInput.type == WHITE_SPACE) currentInput = ReadInput();	

	if(currentInput.value == BlockCommentOpen){
		HandleComments();
	}

	//Consume all white space after comment
	while(currentInput.type == WHITE_SPACE) currentInput = ReadInput();	

	switch(currentInput.type){
		case LETTER:
			return HandleIdentKeyword();	
			break;
		case NUMB:
			return HandleNumber();
			break;
		case END:
			return {.type = EOI};
			break;
		default:
			return HandleSpecialChars();
			break;
	}
}

LexicalToken Lexar::HandleIdentKeyword(){
	string word;
	word.push_back(currentInput.value);

	currentInput = ReadInput();
	while(currentInput.type == NUMB || currentInput.type == LETTER){
		word.push_back(currentInput.value);
		currentInput = ReadInput();
	}

	LexicalToken returnToken;
	returnToken.type = GetKeyWord(word);	

	if(returnToken.type == IDENTIFIER){
		returnToken.identifierName = word;
	}

	return returnToken;
}

LexicalToken Lexar::HandleNumber(){
	LexicalToken returnToken;
	int num;
	num = currentInput.value - '0';

	currentInput = ReadInput();
	while(currentInput.type == NUMB || currentInput.type == LETTER){
		//we found a letter in our number with no space in between
		if(currentInput.type == LETTER){
			//handle octals in here maybe
			returnToken.type = ERR;
			returnToken.identifierName = "Number expected, Character found";
			return returnToken;
		}

		num = num * 10 + (currentInput.value - '0');
		currentInput = ReadInput();
	}

	returnToken.type = NUMBER;
	returnToken.storedNumber = num;

	return returnToken;
}

LexicalToken Lexar::HandleSpecialChars(){
	LexicalToken returnToken;
	int i = 0;

	//Handle simple 1 character special characters;
	while(specialCharacterTable[i].specialCharacter){
		if(currentInput.value == specialCharacterTable[i].specialCharacter){
			//Get currentInput ready for the next thing
			currentInput = ReadInput();

			returnToken.type = specialCharacterTable[i].token;
			return returnToken;
		}
		else i++;
	}

	if(currentInput.value == '<'){
		currentInput = ReadInput();

		if(currentInput.value == '>') returnToken.type = NOTEQUAL;
		else if(currentInput.value == '=') returnToken.type = LESSTHANEQ;
		else returnToken.type = LESSTHAN;

		//Get currentInput ready for the next thing
		if(returnToken.type != LESSTHAN) currentInput = ReadInput();

		return returnToken;
	}
	if(currentInput.value == '>'){
		currentInput = ReadInput();

		if(currentInput.value == '='){
			returnToken.type = GREATERTHANEQ;
			//Get currentInput ready for the next thing
			currentInput = ReadInput();
		} 
		else returnToken.type = GREATERTHAN;

		return returnToken;
	}
	if(currentInput.value == '='){
		currentInput = ReadInput();
		
		if(currentInput.value == '='){
			returnToken.type = EQUAL;
			//Get currentInput ready for the next thing
			currentInput = ReadInput();
		}
		else returnToken.type = ASSIGN;

		return returnToken;
	}
	
	returnToken.type = ERR;
	returnToken.identifierName = "Unexpected token";
	Error("Unexpected token");
	currentInput = ReadInput();
	return returnToken;

}

void Lexar::HandleComments(){
	int startingLine = lineNumber;
	while(currentInput.value != BlockCommentClose){
		currentInput = ReadInput();
		if(currentInput.value == BlockCommentOpen) HandleComments();
		else if(currentInput.type == END) {
			ostringstream oss;
			oss << "Unexpected end of input. Comment was started around line: "
				<< startingLine
				<< " but was never finished.";
			Error(oss.str());
			currentInput = ReadInput();
			return;
		}
	}
	//Consume closing bracket
	currentInput = ReadInput();
}
