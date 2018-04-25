#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"
#include "../src/lexar.h"
#include "../src/parser.h"

TEST_CASE( "Files can be loaded", "[lexar]" ) {
    Lexar lexar = Lexar();
    REQUIRE(lexar.Init("./testPrograms/testSource.txt"));
}

TEST_CASE("Tokenization is successful", "[lexar]"){
    Lexar lexar = Lexar();
    
    SECTION("Handling identifiers"){
        lexar.Init(std::string("this that theother"));
        REQUIRE(lexar.NextToken().type == IDENTIFIER);
        REQUIRE(lexar.NextToken().type == IDENTIFIER);
        REQUIRE(lexar.NextToken().type == IDENTIFIER);
    }
    SECTION("Handling comparisons"){
        lexar.Init(std::string("<> > < <= >="));
        REQUIRE(lexar.NextToken().type == NOTEQUAL);
        REQUIRE(lexar.NextToken().type == GREATERTHAN);
        REQUIRE(lexar.NextToken().type == LESSTHAN);
        REQUIRE(lexar.NextToken().type == LESSTHANEQ);
        REQUIRE(lexar.NextToken().type == GREATERTHANEQ);
    }
    SECTION("Handling parens"){
        lexar.Init(std::string("(this that)"));
        REQUIRE(lexar.NextToken().type == LEFTPAREN);
        REQUIRE(lexar.NextToken().type == IDENTIFIER);
        REQUIRE(lexar.NextToken().type == IDENTIFIER);
        REQUIRE(lexar.NextToken().type == RIGHTPAREN);
    }
    SECTION("Handling keywords"){
        lexar.Init(std::string("if then else begin end"));
        REQUIRE(lexar.NextToken().type == KW_IF);
        REQUIRE(lexar.NextToken().type == KW_THEN);
        REQUIRE(lexar.NextToken().type == KW_ELSE);
        REQUIRE(lexar.NextToken().type == KW_BEGIN);
        REQUIRE(lexar.NextToken().type == KW_END);
    }
    SECTION("Handling comments and operators"){
        lexar.Init(std::string("{comments} + == - * / ~"));
        REQUIRE(lexar.NextToken().type == PLUS);
        REQUIRE(lexar.NextToken().type == EQUAL);
        REQUIRE(lexar.NextToken().type == MINUS);
        REQUIRE(lexar.NextToken().type == TIMES);
        REQUIRE(lexar.NextToken().type == DIVIDE);
    }
    SECTION("Handling Numbers"){
        lexar.Init(std::string("100 500 3 4 29909090"));
        REQUIRE(lexar.NextToken().type == NUMBER);
        REQUIRE(lexar.NextToken().type == NUMBER);
        REQUIRE(lexar.NextToken().type == NUMBER);
        REQUIRE(lexar.NextToken().type == NUMBER);
        REQUIRE(lexar.NextToken().type == NUMBER);
    }
    SECTION("Handling numbers next to identifiers"){
        lexar.Init(std::string("100asdf bob500 alice20bobjoe59"));
        REQUIRE(lexar.NextToken().type == ERR);
        REQUIRE(lexar.NextToken().type == IDENTIFIER);
        REQUIRE(lexar.NextToken().type == IDENTIFIER);
    }
    SECTION("Handling Error"){
        lexar.Init(std::string("~ ! @ # $ % ^ &"));
        REQUIRE(lexar.NextToken().type == ERR);
        REQUIRE(lexar.NextToken().type == ERR);
        REQUIRE(lexar.NextToken().type == ERR);
        REQUIRE(lexar.NextToken().type == ERR);
        REQUIRE(lexar.NextToken().type == ERR);
        REQUIRE(lexar.NextToken().type == ERR);
        REQUIRE(lexar.NextToken().type == ERR);
        REQUIRE(lexar.NextToken().type == ERR);
    }
	SECTION("Just comments"){
		lexar.Init(std::string("{ this is a {test {with } some {nested } comment structures }  in {it}}"));
		REQUIRE(lexar.NextToken().type == EOI);
	}

}


TEST_CASE("Parsing successful", "[parser]"){
    Lexar* lexar = new Lexar();
    Parser parser = Parser(lexar);
    SECTION("Basic addition"){
        lexar->Init(std::string("3 + 4"));
        REQUIRE(parser.Parse());
    }
    SECTION("Basic subtraction"){
        lexar->Init(std::string("3 - 4"));
        REQUIRE(parser.Parse());
    }
    SECTION("Basic mult"){
        lexar->Init(std::string("3 * 4"));
        REQUIRE(parser.Parse());
    }
    SECTION("Basic div"){
        lexar->Init(std::string("3 / 4"));
        REQUIRE(parser.Parse());
    }
    SECTION("Basic paren"){
        lexar->Init(std::string("(3)"));
        REQUIRE(parser.Parse());
    }
    SECTION("Paren"){
        lexar->Init(std::string("(3 + 4 - (3 * 5) / 4)"));
        REQUIRE(parser.Parse());
    }
    SECTION("Error1"){
        lexar->Init(std::string("3 +"));
        REQUIRE(!parser.Parse());
    }
    SECTION("Error2"){
        lexar->Init(std::string("+ 3"));
        REQUIRE(!parser.Parse());
    }
    SECTION("Error3"){
        lexar->Init(std::string("3 + ( "));
        REQUIRE(!parser.Parse());
    }
    SECTION("Error4"){
        lexar->Init(std::string("3 + ( 3 + )"));
        REQUIRE(!parser.Parse());
    }
    SECTION("Error5"){
        lexar->Init(std::string("3 + ( 3 + 5 ) /"));
        REQUIRE(!parser.Parse());
    }
    SECTION("Error6"){
        lexar->Init(std::string("3 + ( ) / 4"));
        REQUIRE(!parser.Parse());
    }
    SECTION("Error7"){
        lexar->Init(std::string("3 + 4 ) / 4"));
        REQUIRE(!parser.Parse());
    }
    SECTION("Error8"){
        lexar->Init(std::string("3 + ( 4 "));
        REQUIRE(!parser.Parse());
    }
}
