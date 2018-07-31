# Semesteral Work 

## Intro

### What is this?

For my programming languages and compilers course I was tasked with writing the front end of a compiler that creates either a GCC AST or LLVM Ir code. I chose LLVM partly because it was newer and seemed to have better documentation. My compiler will take pascal-looking code and spit out LLVM byte code which then gets compiled down to an executable.

### What's in it?

In this project you'll find the lexical analyzer, the parser, the AST, and the IR codegen for that AST.

## Usage

I used cmake to make my project and link llvm but the project can be made by just running make. It compiles to an executable called compiler. Run it to see the usage. It will display an ast and the outputted llvm code. It then writes the bitcode to the filename provided. If you want to do a full compile (compiler -> llc -> gcc) I made a compile.sh that will run each of those taking only the source filename as input.

## Samples

The samples are located in the tests/testPrograms/samples folder. In there you'll find all the given samples plus the two of my own. They can be compiled all together by using the compileAllSamples.sh script in the root directory. It will output the bitcode, objects, and executables into separate folders in the samples directory. 

    - NOTE: Not all of the programs will compile successfully. I did not implement arrays so bubble sort and a few others will fail.

## Grammar

The grammar can be found in the grammer.md, grammer-formal.md and first-follow.md files in the root directory. The grammer.md is simply a simplified version of the grammer-formal.md used to easier understand what was going on from the beginning. It might not be completely up to date.

