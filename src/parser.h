#ifndef PARSER_H
#define PARSER_H

#include "lexar.h"


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
        void Program();
        void ProgramHeader();

        void Block();
        void DeclarationPart();
        void VariableDeclaration();
        void VariableDeclarationPrime();
        void VariableDeclarationPart();
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


