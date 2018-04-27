#include <string>

class AST{
    public:
        virtual ~AST(){};
};

class ProgramAST: public AST{
    private:
        std::string programName;

    public:
        ProgramAST(std::string name){ programName.assign(name); }
};


