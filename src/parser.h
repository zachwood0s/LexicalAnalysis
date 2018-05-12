#ifndef PARSER_H
#define PARSER_H
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

#include "ast.h"
#include "lexar.h"

static llvm::LLVMContext theContext;
static llvm::IRBuilder<> builder(theContext);
static std::unique_ptr<llvm::Module> theModule;
static std::map<std::string, llvm::Value*> namedValues;

class Parser{
    public:
        Parser(Lexar*);
        bool Parse();

    private:
        Lexar* lexar;
        LexicalToken currentToken;
        void Consume(LexicalTokenType type);
        void ConsumeError(LexicalTokenType type);

        //Grammer Handlings
        
        // Main program
        std::unique_ptr<AST> Program();
        std::string ProgramHeader();
        std::unique_ptr<AST> Block();
        std::unique_ptr<AST> DeclarationPart();
        std::unique_ptr<AST> VariableDeclaration();
        std::unique_ptr<AST> VariableDeclarationPrime();
        std::unique_ptr<AST> VariableDeclarationPart();
        std::unique_ptr<AST> ConstantDeclaration();
        std::unique_ptr<AST> ConstantDeclarationPrime();
        std::unique_ptr<AST> ConstantDeclarationPart();
        std::unique_ptr<AST> StatementPart();
        std::unique_ptr<AST> IdentifierList();
        std::unique_ptr<AST> IdentifierListPrime();

        //Procedures
        std::unique_ptr<AST> ProcedureDeclaration();
        std::unique_ptr<AST> ProcedureDeclarationPrime();
        std::unique_ptr<AST> ProcedureHeader();
        std::unique_ptr<AST> ParameterList();
        std::unique_ptr<AST> ParameterListPrime();
        std::unique_ptr<AST> Parameter();

        //Functions
        std::unique_ptr<AST> FunctionDeclaration();
        std::unique_ptr<AST> FunctionDeclarationPrime();
        std::unique_ptr<AST> FunctionHeader();
        std::unique_ptr<AST> Directive();

        //Statements
        std::unique_ptr<AST> StatementSequence();
        std::unique_ptr<AST> StatementSequencePrime();
        std::unique_ptr<AST> Statement();
        std::unique_ptr<AST> ConditionalStatement();
        std::unique_ptr<AST> IfStatment();
        std::unique_ptr<AST> IfStatmentPrime();
        std::unique_ptr<AST> RepeditiveStatement();
        std::unique_ptr<AST> WhileStatement();
        std::unique_ptr<AST> ForStatement();
        std::unique_ptr<AST> ForStatementPrime(std::string, std::unique_ptr<AST>);
        std::unique_ptr<AST> BlockStatment();        
        std::unique_ptr<AST> RegularStatement();
        std::unique_ptr<AST> RegularStatementPrime(std::string);
        std::unique_ptr<AST> AssignmentStatement();
        std::vector<std::unique_ptr<AST>> ProcdureStatement();
        std::vector<std::unique_ptr<AST>> UsageParameterList();
        std::unique_ptr<AST> UsageParameter();

        //Expressions
        std::unique_ptr<AST> Expression();
        std::unique_ptr<AST> ExpressionPrime();
        LexicalTokenType ComparisonOperator();
        std::unique_ptr<AST> BaseExpression();
        std::unique_ptr<AST> BaseExpressionPrime(std::unique_ptr<AST>);
        LexicalTokenType PlusMinusOr();
        std::unique_ptr<AST> Term();
        std::unique_ptr<AST> TermPrime(std::unique_ptr<AST>);
        LexicalTokenType MultDivAnd();
        std::unique_ptr<AST> Factor();

        //MISC
        std::unique_ptr<AST> Type();
};


/* Grammer
 *
 *** If Declaration: ***
 * IF           => IF(EXPRESSION) THEN EXPRESSION F
 * F            => ELIF(EXPRESSION) THEN EXPRESSION F
 * F            => F'
 * F'           => ELSE EXPRESSION
 * F'           => $
 *
 *** Function Call: ***
 *
 * CALL         => IDENTIFIER(P);
 * P            => AP'
 * P'           => ,AP'
 * P'           => $
 * A            => E
 *
 *
 *** Function Definition: ***
 *
 * FUNCTION     => (P) : TYPE; EXPRESSION
 * P            => AP'
 * P'           => ,AP'
 * P'           => $
 * A            => IDENTIFIER: TYPE
 *
 *
 *
 *** Assignment: ***
 *
 * ASSIGNMENT   => IDENTIFIER := EXPRESSION
 *
 *
 *** Expressions: ***
 *
 * EXPRESION    => TE'          | NUMBER, IDENTIFIER, (
 * E'           => +TE'         | +
 * E'           => -TE'         | -
 * E'           => $            | $
 * T            => FT'          | NUMBER, IDENTIFIER, (
 * T'           => *FT'         | *
 * T'           => /FT'         | /
 * T'           => $            | $
 * F            => NUMBER       | NUMBER
 * F            => IDENTIFIER   | IDENTIFIER
 * F            => (E)          | (
 *
 *
 *** Expressions (Revised): ***
 *
 * EXPRESION    => B            | NUMBER, IDENTIFIER, (
 *
 *------- BLOCKS --------
 * B            => BEGIN L END  |
 * B            => L
 * L            => EL';
 * L            => ASSIGNMENT L';
 * L            => IF L'
 * L'           => ;EL'
 * L'           => $
 *----- EXPRESSION ------
 * E            => TE'
 * E'           => +TE'         | +
 * E'           => -TE'         | -
 * E'           => $            | $
 * T            => FT'          | NUMBER, IDENTIFIER, (
 * T'           => *FT'         | *
 * T'           => /FT'         | /
 * T'           => $            | $
 * F            => NUMBER       | NUMBER
 * F            => IDENTIFIER   | IDENTIFIER
 * F            => (E)          | (
 */



#endif


