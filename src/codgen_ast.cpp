#include "ast.h"

#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Value.h"

using namespace llvm;

static llvm::LLVMContext theContext;
static llvm::IRBuilder<> builder(theContext);
static std::unique_ptr<llvm::Module> theModule;
static std::map<std::string, llvm::Value*> namedValues;

Value* LogErrorV(const char *str){
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
}

Value* NumberAST::codegen(){
    return ConstantInt::get(theContext, APSInt(value));
}

Value* VariableIdentifierAST::codegen(){
    Value* v = namedValues[name];
    if(!v)
        LogErrorV("Unknown variable name");
    return v;
}

Value* BinaryOpAST::codegen(){
    Value* L = LHS->codegen();
    Value* R = RHS->codegen();
    if(!L || !R)
        return nullptr;

    switch(op){
        case PLUS: return builder.CreateFAdd(L, R, "addtmp");
        case MINUS: return builder.CreateFSub(L, R, "subtmp");
        case TIMES: return builder.CreateFMul(L, R, "multmp");
        case DIV: return builder.CreateFDiv(L, R, "divtmp");
        default: return LogErrorV("Invalid operator");
    }
}

Value* CallExpessionsAst::codegen(){
    Function* CalleeF = theModule->getFunction(Callee);
    if(!CalleeF)
        return LogErrorV("Unknown function referenced");

    if(CalleeF->arg_size() != Args.size())
        return LogErrorV("Incorrect # arguments passed");

    std::vector<Value*> ArgsV;
    for(int i = 0; i<Args.size(); i++){
        ArgsV.push_back(Args[i]->codegen());
        if(!ArgsV.back())
            return nullptr;
    }
    return builder.CreateCall(CalleeF, ArgsV, "calltmp");
}
