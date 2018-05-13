#ifndef AST_H
#define AST_H

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

struct ValueNamePair{
    int value;   
    std::string name;
};

#define PRINTDPETH(depth, format, ...) {printf("|"); for(int i = 0; i<depth; i++) printf("---"); printf(" "); printf(format, ##__VA_ARGS__);}

class AST {
    public:
        virtual ~AST(){};
        virtual void PrintNode(int depth){};
};

class NumberAST: public AST {
    private:
        int value;

    public:
        NumberAST(int number): value(number){};
        virtual void PrintNode(int depth){
            PRINTDPETH(depth, "Number: %d\n", value);
        }
};

class BinaryOpAST: public AST {
    private:
        LexicalTokenType op;
        std::unique_ptr<AST> LHS, RHS;

    public:
        BinaryOpAST(LexicalTokenType op,
                    std::unique_ptr<AST> LHS,
                    std::unique_ptr<AST> RHS): op(op), LHS(std::move(LHS)), RHS(std::move(RHS)){};

        virtual void PrintNode(int depth){
            PRINTDPETH(depth, "Operator: %s\n", lexicalTokenNames[op]);
            LHS->PrintNode(depth+1);
            RHS->PrintNode(depth+1);
        }
};

class ComparisonOpAST: public AST {
    private:
        LexicalTokenType op;
        std::unique_ptr<AST> LHS, RHS;

    public:
        ComparisonOpAST(LexicalTokenType op,
                    std::unique_ptr<AST> LHS,
                    std::unique_ptr<AST> RHS): op(op), LHS(std::move(LHS)), RHS(std::move(RHS)){};

        virtual void PrintNode(int depth){
            PRINTDPETH(depth, "Comparison: %s\n", lexicalTokenNames[op]);
            LHS->PrintNode(depth+1);
            RHS->PrintNode(depth+1);
        }
};

class VariableIdentifierAST: public AST {
    private:
        std::string name;

    public:
        VariableIdentifierAST(const std::string &name): name(name){}

        virtual void PrintNode(int depth){
            PRINTDPETH(depth, "Variable Identifier %s\n", name.c_str());
        }
};

class VariableDeclarationsOfTypeAST: public AST {
    private:
        LexicalTokenType type;
        std::vector<std::unique_ptr<VariableIdentifierAST>> identifiers;
    public:
        VariableDeclarationsOfTypeAST(std::vector<std::unique_ptr<VariableIdentifierAST>> list,
                                LexicalTokenType type)
            : identifiers(std::move(list)){ this->type = type; };

        virtual void PrintNode(int depth){
            PRINTDPETH(depth, "Declarations of type: %s\n", lexicalTokenNames[type]);
            for(int i = 0; i<identifiers.size(); i++){
                identifiers[i]->PrintNode(depth+1);
            }
        }
};
class VariableDeclarationsAST: public AST {
    private:
        std::vector<std::unique_ptr<AST>> declarations;
    public:
        VariableDeclarationsAST(std::vector<std::unique_ptr<AST>> declarations):declarations(std::move(declarations)){};

        virtual void PrintNode(int depth){
            for(int i = 0; i<declarations.size(); i++){
                declarations[i]->PrintNode(depth);
            }
        }
};


class ConstantDeclarationsAST: public AST {
    private:
        std::vector<ValueNamePair> constants;
    public:
        ConstantDeclarationsAST(std::vector<ValueNamePair> constants): constants(constants){};

        virtual void PrintNode(int depth){
            PRINTDPETH(depth, "Constant Declarations:");
            for(int i = 0; i<constants.size(); i++){
                PRINTDPETH(depth + 1, "\t%s => %d\n", constants[i].name.c_str(), constants[i].value);
            }
        }
};

class CallExpessionsAst: public AST {
    private:
        std::string Callee;
        std::vector<std::unique_ptr<AST>> Args;
    public:
        CallExpessionsAst(const std::string &callee,
                          std::vector<std::unique_ptr<AST>> Args)
            : Callee(callee), Args(std::move(Args)){}

        virtual void PrintNode(int depth){
            PRINTDPETH(depth, "Function Call: %s\n", Callee.c_str());
            PRINTDPETH(depth+1, "Args:\n");
            for(int i = 0; i<Args.size(); i++){
                Args[i]->PrintNode(depth+2);
            }
        }
};

class MainBlockAST: public AST {
    private:
        std::vector<std::unique_ptr<AST>> declarations;
        std::unique_ptr<AST> statementSequence;
    public:
        MainBlockAST(std::vector<std::unique_ptr<AST>> declarations,
                     std::unique_ptr<AST> statementSequence)
            : declarations(std::move(declarations)), 
              statementSequence(std::move(statementSequence)) {};

        virtual void PrintNode(int depth){
            for(int i = 0; i<declarations.size(); i++){
                declarations[i]->PrintNode(depth);
            }
            statementSequence->PrintNode(depth);
        }
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

        virtual void PrintNode(int depth){
            PRINTDPETH(depth, "Prototype: %s\n", name.c_str());
            PRINTDPETH(depth, "Args:\n");
            for(int i = 0; i<Args.size(); i++){
                PRINTDPETH(depth+1, "%s: %s\n", Args[i].name.c_str(), lexicalTokenNames[Args[i].type]);
            }
            if(returnType != EOI)
                PRINTDPETH(depth, "Return Type: %s\n", lexicalTokenNames[returnType]);
        }
};

class FunctionAST: public AST {
    private:
        std::unique_ptr<AST> prototype;
        std::unique_ptr<AST> body;
    
    public:
        FunctionAST(std::unique_ptr<AST> prototype, std::unique_ptr<AST> body)
            : prototype(std::move(prototype)), body(std::move(body)){};

        virtual void PrintNode(int depth){
            PRINTDPETH(depth, "Function Declaration:\n");
            prototype->PrintNode(depth+1);
            PRINTDPETH(depth+1, "Body:\n");
            body->PrintNode(depth+2);
        }
};

class ProgramAST: public AST{
    private:
        std::string programName;
        std::unique_ptr<AST> block;

    public:
        ProgramAST(const std::string &name,
                   std::unique_ptr<AST> block)
            : programName(name), block(std::move(block)){}

        virtual void PrintNode(int depth){
            PRINTDPETH(depth, "Program: %s\n",programName.c_str());
            block->PrintNode(depth);
        }
};

class StatementSequenceAST: public AST{
    private:
        std::vector<std::unique_ptr<AST>> statements;
    public:
        StatementSequenceAST(std::vector<std::unique_ptr<AST>> statements)
            : statements(std::move(statements)){};

        virtual void PrintNode(int depth){
            for(int i = 0; i<statements.size(); i++){
                statements[i]->PrintNode(depth);
            }
        }
};

class IfExpressionAST: public AST{
    private:
        std::unique_ptr<AST> cond, thenPart, elsePart;
    public:
        IfExpressionAST(std::unique_ptr<AST> cond,
                        std::unique_ptr<AST> thenPart,
                        std::unique_ptr<AST> elsePart)
            :cond(std::move(cond)), thenPart(std::move(thenPart)), elsePart(std::move(elsePart)) {}

        virtual void PrintNode(int depth){
            PRINTDPETH(depth,"If Statement:\n");
            PRINTDPETH(depth+1,"Cond:\n");
            cond->PrintNode(depth+2);
            PRINTDPETH(depth+1, "Then:\n");
            thenPart->PrintNode(depth+2);
            if(elsePart != nullptr){
                PRINTDPETH(depth+1,"Else: \n");
                elsePart->PrintNode(depth+2);
            }
        }
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

        virtual void PrintNode(int depth){
            PRINTDPETH(depth, "For Loop:\n");
            PRINTDPETH(depth+1, "Loop Variable: %s\n", loopVarName.c_str());
            PRINTDPETH(depth+1, "Loop Direction: %s\n", lexicalTokenNames[direction]);

            PRINTDPETH(depth+1, "Start Expression: \n");
            start->PrintNode(depth+2);
            PRINTDPETH(depth+1,"End Expression: \n");
            end->PrintNode(depth+2);
            if(step != nullptr){
                PRINTDPETH(depth+1, "Step Expression: \n");
                step->PrintNode(depth+2);
            }
            PRINTDPETH(depth+1, "Body:\n");
            body->PrintNode(depth+2);
        }
};

class WhileExpressionAST: public AST{
    private:
        std::unique_ptr<AST> cond, body;
    public:
        WhileExpressionAST(std::unique_ptr<AST> cond,
                           std::unique_ptr<AST> body)
            :cond(std::move(cond)), body(std::move(body)){}

        virtual void PrintNode(int depth){
            PRINTDPETH(depth, "While Expression:\n");
            PRINTDPETH(depth, "Cond: \n");
            cond->PrintNode(depth+1);
            PRINTDPETH(depth, "Body:\n");
            body->PrintNode(depth+1);
        }
};

#endif


