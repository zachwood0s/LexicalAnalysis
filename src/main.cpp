#include <stdio.h>
#include "lexar.h"
#include "parser.h"
#include "ast.h"

void printSymb(LexicalToken token){
	printf("<%s", lexicalTokenNames[token.type]);

	switch(token.type){
		case IDENTIFIER:
		case ERR:
			printf(" %s", token.identifierName.c_str());
			break;
		case NUMB:
			printf(" %d", token.storedNumber);
			break;
		default:
			break;
	}

	printf(">\n");
}
int main(int argc, char **argv){
	char *fileName;
	printf("Lexical Analysis");
	if(argc == 1){
		printf("Keyboard input, write the source code. \n");
		fileName = NULL;
	}
	else{
		fileName = argv[1];
		printf("Input file %s.\n", fileName);
	}
	Lexar lexar = Lexar();
    Parser parser = Parser(&lexar);
    parser.Parse(); 
	printf("\n\nEnd.\n");


	return 0;
}

