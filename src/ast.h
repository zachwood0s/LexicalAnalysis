#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <stdlib.h>

#include "lexar.h"
#include "llvm/IR/Function.h"

struct TypeNamePair{
    LexicalTokenType type;   
    std::string name;
};

class AST{
    public:
        virtual ~AST(){};
};

class NumberAST: public AST {
    private:
        int value;

    public:
        NumberAST(int number): value(number){};
};

class BinaryOpAST: public AST {
    private:
        LexicalTokenType op;
        std::unique_ptr<AST> LHS, RHS;

    private:
        BinaryOpAST(LexicalTokenType op,
                    std::unique_ptr<AST> LHS,
                    std::unique_ptr<AST> RHS): op(op), LHS(std::move(LHS)), RHS(std::move(RHS)){};
};

class ComparisonOpAST: public AST {
    private:
        LexicalTokenType op;
        std::unique_ptr<AST> LHS, RHS;

    private:
        ComparisonOpAST(LexicalTokenType op,
                    std::unique_ptr<AST> LHS,
                    std::unique_ptr<AST> RHS): op(op), LHS(std::move(LHS)), RHS(std::move(RHS)){};
};

class VariableIdentifierAST: public AST {
    private:
        std::string name;

    public:
        VariableIdentifierAST(const std::string &name): name(name){}
};

class CallExpessionsAst: public AST {
    private:
        std::string Callee;
        std::vector<std::unique_ptr<AST>> Args;
    public:
        CallExpessionsAst(const std::string &callee,
                          std::vector<std::unique_ptr<AST>> Args)
            : Callee(callee), Args(std::move(Args)){}
};

class PrototypeAST: public AST{
    private:
        std::string name;
        std::vector<TypeNamePair> Args;
        LexicalTokenType returnType;

    public:
        PrototypeAST(const std::string &name, std::vector<TypeNamePair> Args, LexicalTokenType returnType)
            : name(name), Args(std::move(Args)){ this->returnType = returnType; };

        const std::string &GetName() const {return name;}
};

class FunctionAST: public AST{
    private:
        std::unique_ptr<PrototypeAST> prototype;
        std::unique_ptr<AST> body;
    
    public:
        FunctionAST(std::unique_ptr<PrototypeAST> prototype, std::unique_ptr<AST> body)
            : prototype(std::move(prototype)), body(std::move(body)){};
};

class ProgramAST: public AST{
    private:
        std::string programName;
        std::unique_ptr<AST> block;

    public:
        ProgramAST(const std::string &name,
                   std::unique_ptr<AST> block)
            : programName(name), block(std::move(block)){}
};

class IfExpressionAST: public AST{
    private:
        std::unique_ptr<AST> cond, thenPart, elsePart;
    public:
        IfExpressionAST(std::unique_ptr<AST> cond,
                        std::unique_ptr<AST> thenPart,
                        std::unique_ptr<AST> elsePart)
            :cond(std::move(cond)), thenPart(std::move(thenPart)), elsePart(std::move(elsePart)) {}
};


