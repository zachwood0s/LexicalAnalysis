#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <stdlib.h>

#include "lexar.h"

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

class VariableIdentifierAST: public AST {
    private:
        std::string name;

    public:
        VariableIdentifierAST(const std::string &name): name(name){}
};

class ProgramAST: public AST{
    private:
        std::string programName;

    public:
        ProgramAST(const std::string &name): programName(name) {}
};


