#include <iostream>
#include <fstream>
#include <stdio.h>
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Bitcode/BitcodeWriter.h"
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
        printf("Usage: compiler [src-path] [output-path]\n");
        return 0;
    }
	fileName = argv[1];
	printf("Input file %s.\n", fileName);
	Lexar lexar = Lexar();
    lexar.Init(fileName);
    Parser parser = Parser(&lexar);
    bool success = parser.Parse(); 
    if(!success) {
        printf("\nParse Error!\nExiting\n");
        return 1;
    }
    parser.tree->PrintNode(0);
    printf("\n\nEnd ast print.\n");
    printf("\nBeginning codegen\n");
    parser.tree->codegen();

    auto theModule = dynamic_cast<ProgramAST*>(parser.tree.get())->GetModule();
    std::error_code error_code;
    std::string bitcodeFilename = argv[2];
    bitcodeFilename+=".bc";
    llvm::StringRef sRefName(bitcodeFilename);
    llvm::raw_fd_ostream raw(sRefName, error_code,  (llvm::sys::fs::OpenFlags)8);
    llvm::WriteBitcodeToFile(*theModule, raw);

/*    std::string gccComand = "gcc ";
    gccComand+=argv[2];
    gccComand+=".o -o ";
    gccComand+=argv[2];
    printf("Executing command: %s\n", gccComand.c_str());
    std::system(gccComand.c_str());*/

/*
    auto theModule = dynamic_cast<ProgramAST*>(parser.tree.get())->GetModule();
    // Initialize the target registry etc.
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    theModule->setTargetTriple(TargetTriple);

    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

    // Print an error and exit if we couldn't find the requested target.
    // This generally occurs if we've forgotten to initialise the
    // TargetRegistry or we have a bogus target triple.
    if (!Target) {
        llvm::errs() << Error;
        return 1;
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto TheTargetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    theModule->setDataLayout(TheTargetMachine->createDataLayout());

    auto Filename = argv[2];
    std::error_code EC;
    llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::F_None);

    if (EC) {
        llvm::errs() << "Could not open file: " << EC.message();
        return 1;
    }

    llvm::legacy::PassManager pass;
    auto FileType = llvm::TargetMachine::CGFT_ObjectFile;

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, FileType)) {
        llvm::errs() << "TheTargetMachine can't emit a file of this type";
        return 1;
    }

    pass.run(*theModule.get());
    dest.flush();

    llvm::outs() << "Wrote " << Filename << "\n";
    */
    return 0;

}

