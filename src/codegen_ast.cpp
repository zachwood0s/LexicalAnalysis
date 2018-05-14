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
static std::map<std::string, AllocaInst*> namedValues;

static AllocaInst *CreateEntryBlockAlloca(Function *theFunction,
        const std::string &VarName){
    IRBuilder<> TmpB(&theFunction->getEntryBlock(),
            theFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(Type::getDoubleTy(theContext), 0, VarName.c_str());
}

Value* LogErrorV(const char *str){
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
}


Value* MainBlockAST::codegen(){
    //Remember I want to call the DoAllocations on the declarations not code gen. will need to cast
}

Value* ProgramAST::codegen(){
   theModule = llvm::make_unique<Module>(programName, theContext);
   return block->codegen();
}

Value* StatementSequenceAST::codegen(){

}

Value* NumberAST::codegen(){
    return ConstantInt::get(theContext, APSInt(value));
}

Value* VariableIdentifierAST::codegen(){
    Value* v = namedValues[name];
    if(!v)
        LogErrorV("Unknown variable name");

    return builder.CreateLoad(v, name.c_str());
}

Value* UnaryOpAST::codegen(){

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
        case ASSIGN:
            {
                VariableIdentifierAST *LHSE = dynamic_cast<VariableIdentifierAST*>(LHS.get());
                if(!LHSE)
                    return LogErrorV("left hand side of assignment must be a varaible");

                Value *Variable = namedValues[LHSE->GetName()];
                if(!Variable)
                    return LogErrorV("Unknown variable name");

                builder.CreateStore(R, Variable);
                return R;
            }
        default: return LogErrorV("Invalid operator");
    }
}

Value* ComparisonOpAST::codegen(){

}

std::vector<AllocaInst*> VariableDeclarationsOfTypeAST::DoAllocations(){
    std::vector<AllocaInst *> OldBindings;

    Function *TheFunction = builder.GetInsertBlock()->getParent();

    for(int i = 0; i<this->identifiers.size(); i++){
        const std::string &VarName = identifiers[i]->GetName();
        Value *InitVal = ConstantFP::get(theContext, APFloat(0.0));

        AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, VarName);
        builder.CreateStore(InitVal, Alloca);

        OldBindings.push_back(namedValues[VarName]);

        namedValues[VarName] = Alloca;
    }

    //Not sure if I want to return a map with the name and the old binding.
    //Im gonna try resetting the namedValue by doing:
    //if(Alloca)
    //  namedValues[Alloca->getName()] = Alloca;
    //
    //That's not what they did in the tutorial but I think it should work
    return OldBindings;
    //TODO Im tired and want to go to bed
    //but the problem is in the tutorial they call the body gen code
    //from in here and then reset the vars but in mine it 
    //wont happen that way. I think what I need
    //to do is return the old vars back out so that they can be reset after the body
    //code is created.
}

std::vector<AllocaInst *> VariableDeclarationsAST::DoAllocations(){
    std::vector<AllocaInst *> OldBindings;
    for(auto &Decl : this->declarations){
        auto old = dynamic_cast<DeclarationAST*>(Decl.get())->DoAllocations();
        OldBindings.insert(OldBindings.end(), old.begin(), old.end());
    }
    return OldBindings;
}

std::vector<AllocaInst *> ConstantDeclarationsAST::DoAllocations(){

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

Value* IfExpressionAST::codegen(){
    Value *CondV = cond->codegen();
    if(!CondV)
        return nullptr;

    CondV = builder.CreateFCmpONE(CondV, ConstantFP::get(theContext, APFloat(0.0)), "ifcond");
    Function *theFunction = builder.GetInsertBlock()->getParent();
    BasicBlock *thenBB = BasicBlock::Create(theContext, "then", theFunction);
    BasicBlock *elseBB = BasicBlock::Create(theContext, "else");
    BasicBlock *mergeBB = BasicBlock::Create(theContext, "ifcont");

    builder.CreateCondBr(CondV, thenBB, elseBB);

    builder.SetInsertPoint(thenBB);
    Value *ThenV = thenPart->codegen();
    if(!ThenV)
        return nullptr;

    builder.CreateBr(mergeBB);
    thenBB = builder.GetInsertBlock();

    theFunction->getBasicBlockList().push_back(elseBB);
    builder.SetInsertPoint(elseBB);

    Value *ElseV = elsePart->codegen();
    if(!ElseV)
        return nullptr;

    builder.CreateBr(mergeBB);
    elseBB = builder.GetInsertBlock();
    
    theFunction->getBasicBlockList().push_back(mergeBB);
    builder.SetInsertPoint(mergeBB);
    PHINode *PN = builder.CreatePHI(Type::getDoubleTy(theContext), 2, "iftmp");

    PN->addIncoming(ThenV, thenBB);
    PN->addIncoming(ElseV, elseBB);

    return PN;
}

Value* ForExpressionAST::codegen(){
    Value *StartVal = start->codegen();
    if(!StartVal)
        return nullptr;

    Function *TheFunction = builder.GetInsertBlock()->getParent();

    AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, loopVarName);

    builder.CreateStore(StartVal, Alloca);

    BasicBlock *LoopBB = BasicBlock::Create(theContext, "loop", TheFunction);

    builder.CreateBr(LoopBB);

    builder.SetInsertPoint(LoopBB);

    AllocaInst *OldVal = namedValues[loopVarName];
    namedValues[loopVarName] = Alloca;

    if(!body->codegen())
        return nullptr;

    Value *StepVal = nullptr;
    if(step){
        StepVal = step->codegen();
        if(!StepVal)
            return nullptr;
    }
    else{
        StepVal = ConstantFP::get(theContext, APFloat(1.0));
    }
    Value *CurVar = builder.CreateLoad(Alloca, loopVarName.c_str());
    Value *NextVar = builder.CreateFAdd(CurVar, StepVal, "nextvar");
    builder.CreateStore(NextVar, Alloca);

    Value *EndCond = end->codegen();
    if(!EndCond)
        return nullptr;

    EndCond = builder.CreateFCmpONE(EndCond, ConstantFP::get(theContext, APFloat(0.0)), "loopcond");

    BasicBlock *AfterBB = BasicBlock::Create(theContext, "afterloop", TheFunction);
    builder.CreateCondBr(EndCond, LoopBB, AfterBB);
    builder.SetInsertPoint(AfterBB);

    if(OldVal)
        namedValues[loopVarName] = OldVal;
    else
        namedValues.erase(loopVarName);

    return Constant::getNullValue(Type::getDoubleTy(theContext));
}

Value* WhileExpressionAST::codegen(){

}

Value* PrototypeAST::codegen(){
    std::vector<Type*> Ints(Args.size(), Type::getInt64Ty(theContext));

    FunctionType *FT = FunctionType::get(Type::getDoubleTy(theContext), Ints, false);

    Function *F = Function::Create(FT, Function::ExternalLinkage, name, theModule.get());

    unsigned Idx = 0;
    for(auto &Arg : F->args()){
        Arg.setName(Args[Idx++].name);
    }

    return F;
}

Value* FunctionAST::codegen(){
    PrototypeAST *proto = dynamic_cast<PrototypeAST*>(prototype.get());
    Function *theFunction = theModule->getFunction((proto->GetName()));

    if(!theFunction)
        theFunction = static_cast<Function*>(proto->codegen());

    if(!theFunction)
        return nullptr;

    if(!theFunction->empty())
        return LogErrorV("Function cannot be redefined");

    BasicBlock *BB = BasicBlock::Create(theContext, "entry", theFunction);
    builder.SetInsertPoint(BB);

    namedValues.clear();
    for(auto &Arg : theFunction->args()){
        AllocaInst *Alloca = CreateEntryBlockAlloca(theFunction, Arg.getName());

        builder.CreateStore(&Arg, Alloca);

        namedValues[Arg.getName()] = Alloca;
    }

    if(Value *RetVal = body->codegen()){
        builder.CreateRet(RetVal);
        verifyFunction(*theFunction);
        return theFunction;
    }
    theFunction->eraseFromParent();
    return nullptr;
}
