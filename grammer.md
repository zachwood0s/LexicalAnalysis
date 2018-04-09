# Grammer

## Main Program

Program
    - Program-Header Block

Program-Header
    - *program* Identifier

Block
    - Declaration-Part Statement-Part

Declaration-Part
    - [Variable-Declaration]
    - [Constant-Declaration]
    - [Procedure-Declaration]

Variable-Declaration
    - *var* Identifier-List *:* Type*;* {Variable-Declaration*;*}

Identifier-List
    - Identifier {*,* Identifier}

Constant-Declaration
    - *const* Identifier *=* Constant*;* {Identifier *=* Constant*;*}

Statement-Part
    - *begin* Statement-Sequence *end*

## Procedures

Procedure-Declaration
    - *procedure* FunctionIdentifier Parameter-List [*:* Type]*;*

Parameter-List
    - Parameter {*,* Parameter}

Parameter
    - Identifier *:* Type

## Statements

Statement-Sequence
    - Statement {*;* Statement}

Statement
    - Conditional-Statement | Repeditive-Statement | Block-Statement

Conditional-Statement
    - If-Statement

If-Statement
    - *if* Expression *then* Statement [*else* Statement]

Repeditive-Statement
    - While-Statement | For-Statement

While-Statement
    - *while* Expression *do* Statement

For-Statement
    - *for* Identifier *:=* Expression (*to*|*downto*) Expression *do* Statement

Block-Statement
    - *begin* Statement-Sequence *end*

## Expressions

Expression
    - Base-Expression [Comparison-Operator Base-Expression]

Comparison-Operator
    - *=* | *<* | *>* | *<=* | *>=* | *<>*

Base-Expression
    - Term {Plus-Minus-Or Term}

Term
    - Factor {Mult-Div-And Factor}

Factor
    - Identifier | Number
    - *(* Expression *)*

## Others

Type
    - *integer* | Array-Type

Array-Type
    - *array* *[* Expression *..* Expression *]* *of* Type;
