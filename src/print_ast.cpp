#include "ast.h"

// Print functions
void MainBlockAST::PrintNode(int depth){
    for(int i = 0; i<declarations.size(); i++){
        declarations[i]->PrintNode(depth);
    }
    statementSequence->PrintNode(depth);
}

void ProgramAST::PrintNode(int depth){
    PRINTDPETH(depth, "Program: %s\n",programName.c_str());
    for(int i = 0; i<declarations.size(); i++){
        declarations[i]->PrintNode(depth);
    }
    statementSequence->PrintNode(depth);
}

void StatementSequenceAST::PrintNode(int depth){
    for(int i = 0; i<statements.size(); i++){
        statements[i]->PrintNode(depth);
    }
}

void NumberAST::PrintNode(int depth){
    PRINTDPETH(depth, "Number: %d\n", value);
}

void VariableIdentifierAST::PrintNode(int depth){
    PRINTDPETH(depth, "Variable Identifier %s\n", name.c_str());
}

void UnaryOpAST::PrintNode(int depth){
    PRINTDPETH(depth, "Unary Operator: %s\n", lexicalTokenNames[op]);
    expression->PrintNode(depth+1);
}

void BinaryOpAST::PrintNode(int depth){
    PRINTDPETH(depth, "Operator: %s\n", lexicalTokenNames[op]);
    LHS->PrintNode(depth+1);
    RHS->PrintNode(depth+1);
}

void ComparisonOpAST::PrintNode(int depth){
    PRINTDPETH(depth, "Comparison: %s\n", lexicalTokenNames[op]);
    LHS->PrintNode(depth+1);
    RHS->PrintNode(depth+1);
}

void ExitBreakStatementAST::PrintNode(int depth){
    PRINTDPETH(depth, "%s\n", lexicalTokenNames[exitOrBreak]);
}

void VariableDeclarationsOfTypeAST::PrintNode(int depth){
    PRINTDPETH(depth, "Variable declarations of type: %s\n", lexicalTokenNames[type]);
    for(int i = 0; i<identifiers.size(); i++){
        identifiers[i]->PrintNode(depth+1);
    }
}

void VariableDeclarationsAST::PrintNode(int depth){
    for(int i = 0; i<declarations.size(); i++){
        declarations[i]->PrintNode(depth);
    }
}

void ConstantDeclarationsAST::PrintNode(int depth){
    PRINTDPETH(depth, "Constant Declarations:\n");
    for(int i = 0; i<constants.size(); i++){
        PRINTDPETH(depth + 1, "%s => %d\n", constants[i].name.c_str(), constants[i].value);
    }
}

void CallExpessionsAst::PrintNode(int depth){
    PRINTDPETH(depth, "Function Call: %s\n", Callee.c_str());
    PRINTDPETH(depth+1, "Args:\n");
    for(int i = 0; i<Args.size(); i++){
        Args[i]->PrintNode(depth+2);
    }
}

void IfExpressionAST::PrintNode(int depth){
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

void ForExpressionAST::PrintNode(int depth){
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

void WhileExpressionAST::PrintNode(int depth){
    PRINTDPETH(depth, "While Expression:\n");
    PRINTDPETH(depth+1, "Cond: \n");
    cond->PrintNode(depth+1);
    PRINTDPETH(depth+1, "Body:\n");
    body->PrintNode(depth+1);
}

void PrototypeAST::PrintNode(int depth){
    PRINTDPETH(depth, "Prototype: %s\n", name.c_str());
    PRINTDPETH(depth, "Args:\n");
    for(int i = 0; i<Args.size(); i++){
        PRINTDPETH(depth+1, "%s: %s\n", Args[i].name.c_str(), lexicalTokenNames[Args[i].type]);
    }
    if(returnType != EOI)
        PRINTDPETH(depth, "Return Type: %s\n", lexicalTokenNames[returnType]);
}

void FunctionAST::PrintNode(int depth){
    PRINTDPETH(depth, "Function Declaration:\n");
    prototype->PrintNode(depth+1);
    PRINTDPETH(depth+1, "Body:\n");
    body->PrintNode(depth+2);
}

