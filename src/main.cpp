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
int main(int argc, char *argv[]){
	char *fileName;

    if(argc != 3){
        printf("Usage: simple-tool [src-path] [output-path]\n");
        return 0;
    }
	fileName = argv[1];
	printf("Input file %s.\n", fileName);
	Lexar lexar = Lexar();
    lexar.Init(fileName);
    Parser parser = Parser(&lexar);
    parser.Parse(); 
    parser.tree->PrintNode(0);
	printf("\n\nEnd ast print.\n");
    printf("\nBeginning codegen\n");
    parser.tree->codegen();
	return 0;
}

