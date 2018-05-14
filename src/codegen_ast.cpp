#include "ast.h"

#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/APFloat.h"
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
    return TmpB.CreateAlloca(Type::getInt64Ty(theContext), 0, VarName.c_str());
}

Value* LogErrorV(const char *str){
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
}


Value* MainBlockAST::codegen(){
    //Remember I want to call the DoAllocations on the declarations not code gen. will need to cast
    std::vector<AllocaInst *> OldBindings;
    for(int i = 0; i<declarations.size(); i++){
        auto decl = dynamic_cast<DeclarationAST*>(declarations[i].get());
        if(!decl){
            printf("DeclarationAST cast failed");
            return nullptr;
        }
        if(PrototypeAST *proto = dynamic_cast<PrototypeAST*>(decl)){
            proto->codegen();
        }
        else{
            auto oldBind = decl->DoAllocations();
            OldBindings.insert(OldBindings.end(), oldBind.begin(), oldBind.end());
        }
    }
   
    Value* BodyVal = statementSequence->codegen();
    if(!BodyVal)
        return nullptr;

    for(int i = 0; i<OldBindings.size();i++){
        //This won't work cuz it allows for vars to be accessed from an already deleted scope
        if(OldBindings[i])
            namedValues[OldBindings[i]->getName()] = OldBindings[i];
    }

    return BodyVal;
}

Value* ProgramAST::codegen(){
    theModule = llvm::make_unique<Module>(programName, theContext);

    FunctionType *FT = FunctionType::get(Type::getVoidTy(theContext), false);
    Function *F = Function::Create(FT, Function::ExternalLinkage, "__mainprogram__", theModule.get());
    BasicBlock *BB = BasicBlock::Create(theContext, "entry", F);
    builder.SetInsertPoint(BB);

    for(int i = 0; i<declarations.size(); i++){
        auto decl = dynamic_cast<DeclarationAST*>(declarations[i].get());
        if(!decl){
            printf("DeclarationAST cast failed");
            return nullptr;
        }
        if(PrototypeAST *proto = dynamic_cast<PrototypeAST*>(decl)){
            proto->codegen();
        }
        else{
            auto oldBind = decl->DoAllocations();
        }
    }
    builder.SetInsertPoint(BB);
    Value* BodyVal = statementSequence->codegen();
    if(!BodyVal)
        return nullptr;
    theModule->print(errs(), nullptr);
    return BodyVal;
}

Value* StatementSequenceAST::codegen(){
    for(int i = 0; i<statements.size()-1; i++){
        statements[i]->codegen();
    }
    return statements[statements.size()-1]->codegen();
}

Value* NumberAST::codegen(){
    return ConstantInt::get(theContext, APSInt(64, value));
}

Value* VariableIdentifierAST::codegen(){
    Value* v = namedValues[name];
    if(!v)
        LogErrorV("Unknown variable name");

    return builder.CreateLoad(v, name.c_str());
}

Value* UnaryOpAST::codegen(){
    return expression->codegen();
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
                if(!Variable){
                    printf("Unknown variable name %s\n", LHSE->GetName().c_str()); 
                    return nullptr;
                }


                builder.CreateStore(R, Variable);
                return R;
            }
        default:{printf("Invalid operator %s\n", lexicalTokenNames[op]); return nullptr; } 
    }
}

Value* ComparisonOpAST::codegen(){
    Value* L = LHS->codegen();
    Value* R = RHS->codegen();
    if(!L || !R)
        return nullptr;

    switch(op){    
            //L = Builder.CreateFCmpULT(L, R, "cmptmp");
            //return Builder.CreateUIToFP(L, Type::getDoubleTy(TheContext), "booltmp");
        case LESSTHAN: return builder.CreateICmpULT(L, R, "cmptmp");
        case LESSTHANEQ: return builder.CreateICmpULE(L, R, "cmptmp");
        case GREATERTHAN: return builder.CreateICmpUGT(L, R, "cmptmp");
        case GREATERTHANEQ: return builder.CreateICmpUGE(L, R, "cmptmp");
        case NOTEQUAL: return builder.CreateICmpNE(L, R, "cmptmp");
        case EQUAL: return builder.CreateICmpEQ(L, R, "cmptmp");
        default:{printf("Invalid comparison operator %s\n", lexicalTokenNames[op]); return nullptr; } 
    }
}

std::vector<AllocaInst*> VariableDeclarationsOfTypeAST::DoAllocations(){
    std::vector<AllocaInst *> OldBindings;

    Function *TheFunction = builder.GetInsertBlock()->getParent();

    for(int i = 0; i<this->identifiers.size(); i++){
        const std::string &VarName = identifiers[i]->GetName();
        Value *InitVal = ConstantInt::get(theContext, APSInt(64, 0));

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
    return {};
}

Value* CallExpessionsAst::codegen(){
    Function* CalleeF = theModule->getFunction(Callee);
    if(!CalleeF){
        printf("Unknown function referenced: %s\n", Callee.c_str());
        return nullptr;
    }

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

    CondV = builder.CreateICmpNE(CondV, ConstantInt::get(theContext, APInt(1, 0)), "ifcond");
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

    Value *ElseV;
    if(elsePart)
        ElseV = elsePart->codegen();
    else
        ElseV = Constant::getNullValue(Type::getInt64Ty(theContext));

    if(!ElseV)
        return nullptr;

    builder.CreateBr(mergeBB);
    elseBB = builder.GetInsertBlock();
    
    theFunction->getBasicBlockList().push_back(mergeBB);
    builder.SetInsertPoint(mergeBB);
    PHINode *PN = builder.CreatePHI(Type::getInt64Ty(theContext), 2, "iftmp");

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
        StepVal = ConstantInt::get(theContext, APSInt(64, 1));
    }
    Value *CurVar = builder.CreateLoad(Alloca, loopVarName.c_str());
    Value *NextVar = builder.CreateFAdd(CurVar, StepVal, "nextvar");
    builder.CreateStore(NextVar, Alloca);

    Value *EndCond = end->codegen();
    if(!EndCond)
        return nullptr;

    EndCond = builder.CreateFCmpONE(EndCond, ConstantInt::get(theContext, APSInt(64, 0)), "loopcond");

    BasicBlock *AfterBB = BasicBlock::Create(theContext, "afterloop", TheFunction);
    builder.CreateCondBr(EndCond, LoopBB, AfterBB);
    builder.SetInsertPoint(AfterBB);

    if(OldVal)
        namedValues[loopVarName] = OldVal;
    else
        namedValues.erase(loopVarName);

    return Constant::getNullValue(Type::getInt64Ty(theContext));
}

Value* WhileExpressionAST::codegen(){
    return nullptr;
}

Value* PrototypeAST::codegen(){
    std::vector<Type*> Ints(Args.size(), Type::getInt64Ty(theContext));

    FunctionType *FT = FunctionType::get(Type::getInt64Ty(theContext), Ints, false);

    Function *F = Function::Create(FT, Function::ExternalLinkage, name, theModule.get());

    unsigned Idx = 0;
    for(auto &Arg : F->args()){
        Arg.setName(Args[Idx++].name);
    }

    return F;
}

std::vector<AllocaInst*> FunctionAST::DoAllocations(){
    PrototypeAST *proto = dynamic_cast<PrototypeAST*>(prototype.get());
    Function *theFunction = theModule->getFunction((proto->GetName()));

    if(!theFunction)
        theFunction = static_cast<Function*>(proto->codegen());

    if(!theFunction)
        return {};

    if(!theFunction->empty()){
        printf("Function %s cannot be redefined\n", proto->GetName().c_str());
        return {};
    }

    BasicBlock *BB = BasicBlock::Create(theContext, "entry", theFunction);
    builder.SetInsertPoint(BB);

    std::vector<AllocaInst *> OldBindings;
    //namedValues.clear();
    for(auto &Arg : theFunction->args()){
        AllocaInst *Alloca = CreateEntryBlockAlloca(theFunction, Arg.getName());

        builder.CreateStore(&Arg, Alloca);

        OldBindings.push_back(namedValues[Arg.getName()]);
        namedValues[Arg.getName()] = Alloca;
    }

    if(Value *RetVal = body->codegen()){
        //builder.CreateRetVoid();
        builder.CreateRet(RetVal);
        verifyFunction(*theFunction);
        return OldBindings;
    }
    theFunction->eraseFromParent();
    return {};
}
