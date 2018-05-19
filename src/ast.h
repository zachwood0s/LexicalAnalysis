#ifndef AST_H
#define AST_H

#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include "llvm/IR/Value.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"

#include "lexar.h"


struct TypeNamePair{
    LexicalTokenType type;   
    std::string name;
};

struct ValueNamePair{
    int value;   
    std::string name;
};

#define PRINTDPETH(depth, format, ...) {printf("|"); for(int i = 0; i<depth; i++) printf("---"); printf(" "); printf(format, ##__VA_ARGS__);}

class AST {
    public:
        virtual ~AST(){};
        virtual void PrintNode(int depth){};
        virtual llvm::Value* codegen() = 0;
        
};

// Main Parts

class MainBlockAST: public AST {
    private:
        std::vector<std::unique_ptr<AST>> declarations;
        std::unique_ptr<AST> statementSequence;
    public:
        MainBlockAST(std::vector<std::unique_ptr<AST>> declarations,
                     std::unique_ptr<AST> statementSequence)
            : declarations(std::move(declarations)), 
              statementSequence(std::move(statementSequence)) {};

        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
};

class ProgramAST: public AST{
    private:
        std::string programName;
        std::vector<std::unique_ptr<AST>> declarations;
        std::unique_ptr<AST> statementSequence;
        std::unique_ptr<llvm::Module> llvmModule;

    public:
        std::unique_ptr<llvm::Module> GetModule(){return std::move(llvmModule);};
        ProgramAST(const std::string &name,
                   std::vector<std::unique_ptr<AST>> declarations,
                     std::unique_ptr<AST> statementSequence)
            : programName(name), 
              declarations(std::move(declarations)), 
              statementSequence(std::move(statementSequence)){};

        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
};

class StatementSequenceAST: public AST{
    private:
        std::vector<std::unique_ptr<AST>> statements;
    public:
        StatementSequenceAST(std::vector<std::unique_ptr<AST>> statements)
            : statements(std::move(statements)){};

        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
};

// Numbers and Identifiers

class NumberAST: public AST {
    private:
        int value;

    public:
        NumberAST(int number): value(number){};

        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
};

class VariableIdentifierAST: public AST {
    private:
        std::string name;

    public:
        VariableIdentifierAST(const std::string &name): name(name){}

        std::string GetName(){return name;};
        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
};

// Operators

class UnaryOpAST: public AST {
    private:
        LexicalTokenType op;
        std::unique_ptr<AST> expression;

    public:
        UnaryOpAST(LexicalTokenType op,
                   std::unique_ptr<AST> expression )
            : op(op), expression(std::move(expression)){};

        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
};

class BinaryOpAST: public AST {
    private:
        LexicalTokenType op;
        std::unique_ptr<AST> LHS, RHS;

    public:
        BinaryOpAST(LexicalTokenType op,
                    std::unique_ptr<AST> LHS,
                    std::unique_ptr<AST> RHS): op(op), LHS(std::move(LHS)), RHS(std::move(RHS)){};

        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
};

class ComparisonOpAST: public AST {
    private:
        LexicalTokenType op;
        std::unique_ptr<AST> LHS, RHS;

    public:
        ComparisonOpAST(LexicalTokenType op,
                    std::unique_ptr<AST> LHS,
                    std::unique_ptr<AST> RHS): op(op), LHS(std::move(LHS)), RHS(std::move(RHS)){};

        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
};

class ExitBreakStatementAST: public AST{
    private:
        LexicalTokenType exitOrBreak;

    public:
        ExitBreakStatementAST(LexicalTokenType exitOrBreak): exitOrBreak(exitOrBreak){};
        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
};

// Declarations

class DeclarationAST : public AST{
    public:
        virtual std::vector<llvm::AllocaInst*> DoAllocations() = 0;
};

class VariableDeclarationsOfTypeAST: public DeclarationAST {
    private:
        LexicalTokenType type;
        std::vector<std::unique_ptr<VariableIdentifierAST>> identifiers;
    public:
        VariableDeclarationsOfTypeAST(std::vector<std::unique_ptr<VariableIdentifierAST>> list,
                                LexicalTokenType type)
            : identifiers(std::move(list)){ this->type = type; };

        void PrintNode(int depth) override;
        llvm::Value* codegen() override {return nullptr;};
        std::vector<llvm::AllocaInst*> DoAllocations() override;
};

class VariableDeclarationsAST: public DeclarationAST {
    private:
        std::vector<std::unique_ptr<AST>> declarations;
    public:
        VariableDeclarationsAST(std::vector<std::unique_ptr<AST>> declarations):declarations(std::move(declarations)){};

        void PrintNode(int depth) override;
        llvm::Value* codegen() override {return nullptr;};
        std::vector<llvm::AllocaInst*> DoAllocations() override;
};


class ConstantDeclarationsAST: public DeclarationAST {
    private:
        std::vector<ValueNamePair> constants;
    public:
        ConstantDeclarationsAST(std::vector<ValueNamePair> constants): constants(constants){};

        void PrintNode(int depth) override;
        llvm::Value* codegen() override {return nullptr;};
        std::vector<llvm::AllocaInst*> DoAllocations() override;
};

// Expressions

class CallExpessionsAst: public AST {
    private:
        std::string Callee;
        std::vector<std::unique_ptr<AST>> Args;
    public:
        CallExpessionsAst(const std::string &callee,
                          std::vector<std::unique_ptr<AST>> Args)
            : Callee(callee), Args(std::move(Args)){}

        CallExpessionsAst(const std::string &callee)
            : Callee(callee){}

        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
};

class IfExpressionAST: public AST{
    private:
        std::unique_ptr<AST> cond, thenPart, elsePart;
    public:
        IfExpressionAST(std::unique_ptr<AST> cond,
                        std::unique_ptr<AST> thenPart,
                        std::unique_ptr<AST> elsePart)
            :cond(std::move(cond)), thenPart(std::move(thenPart)), elsePart(std::move(elsePart)) {}

        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
};

class ForExpressionAST: public AST{
    private:
        std::string loopVarName;
        LexicalTokenType direction; //IE: TO or DOWNTO
        std::unique_ptr<AST> start, end, step, body;
    public:
        ForExpressionAST(std::string loopVarName,
                         LexicalTokenType direction,
                         std::unique_ptr<AST> start,
                         std::unique_ptr<AST> end,
                         std::unique_ptr<AST> step,
                         std::unique_ptr<AST> body)
            :loopVarName(std::move(loopVarName)), start(std::move(start)), end(std::move(end)),step(std::move(step)), body(std::move(body)){ this->direction = direction; }

        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
};

class WhileExpressionAST: public AST{
    private:
        std::unique_ptr<AST> cond, body;
    public:
        WhileExpressionAST(std::unique_ptr<AST> cond,
                           std::unique_ptr<AST> body)
            :cond(std::move(cond)), body(std::move(body)){}

        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
};

// Functions, Prototypes, and Procedures

class PrototypeAST: public DeclarationAST{
    private:
        std::string name;
        std::vector<TypeNamePair> Args;
        LexicalTokenType returnType;

    public:
        PrototypeAST(const std::string &name, std::vector<TypeNamePair> Args, LexicalTokenType returnType)
            : name(name), Args(std::move(Args)){ this->returnType = returnType; };

        const std::string &GetName() const {return name;}
        const LexicalTokenType GetReturnType() const {return returnType;}

        void PrintNode(int depth) override;
        llvm::Value* codegen() override;
        std::vector<llvm::AllocaInst*> DoAllocations() override {return{};};
};

class FunctionAST: public DeclarationAST {
    private:
        std::unique_ptr<AST> prototype;
        std::unique_ptr<AST> body;
    
    public:
        FunctionAST(std::unique_ptr<AST> prototype, std::unique_ptr<AST> body)
            : prototype(std::move(prototype)), body(std::move(body)){};

        void PrintNode(int depth) override;
        llvm::Value* codegen() override { return nullptr; };
        std::vector<llvm::AllocaInst*> DoAllocations() override;
};

#endif


