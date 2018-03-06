#include <stdio.h>
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
	inputFile = new istream(NULL);
}
Lexar::~Lexar(){
	if(inputFile != &std::cin){
		delete inputFile;
	}
	inputFile = NULL;
}

bool Lexar::Init(char* fileName){
	if(!fileName){
		inputFile = &std::cin;
	}
	else{
		inputFile = new ifstream(fileName);
		if(!inputFile){
			printf("Error opening %s \n",fileName);
			return false;
		}
	}
	currentInput = ReadInput();
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
	char next = inputFile->get();
	if(next == '\n') lineNumber++;
	return next;
}

InputToken Lexar::ReadInput() {
	int character = GetNextChar();
	InputToken input;
	input.value = character;
	if ((character>='A' && character<='Z') || (character>='a' && character<='z')){
	  input.type = LETTER;	
	}
	else if (character>='0' && character<='9'){
	  input.type = NUMB;
	}
	else if (character == EOF){
	  input.type = END;
	}
	else if (character <= ' '){
	  input.type = WHITE_SPACE;
	}
	else{
	  input.type = NO_TYPE;
	}
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
	LexicalToken returnToken;
	string word;
	word.push_back(currentInput.value);

	currentInput = ReadInput();
	while(currentInput.type == NUMB || currentInput.type == LETTER){
		word.push_back(currentInput.value);
		currentInput = ReadInput();
	}

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
	
	returnToken.type = ERR;
	returnToken.identifierName = "Unexpected token";
	return returnToken;

}

//TODO This isn't working the way I want it to. It's very close but
//the comments are a little off. I think it has something to do with
//consuming to few or to many opening/closing braces
void Lexar::HandleComments(){
	while(currentInput.value != BlockCommentClose){
		currentInput = ReadInput();
		if(currentInput.value == BlockCommentOpen) HandleComments();
		else if(currentInput.type == END) printf("error, unexpected end of input");
	}
	//Consume closing bracket
	currentInput = ReadInput();
}
