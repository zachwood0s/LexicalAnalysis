
# Formal Grammar

## Non-Terminals are written in <name>

## Main Program
                                    First                        Follow
<Program>                    | program            
<Program-Header>             | program           

<Block>                      | var, const, procedure, begin
<Declaration-Part>           | var                               begin
<Declaration-Part>           | const           
<Declaration-Part>           | procedure
<Declaration-Part>           | $

<Variable-Declaration>       | var                               FOLLOW(<Declaration-Part>)
<Variable-Declaration-Pr>    | ;                                 FOLLOW(<Variable-Declaration>)
<Variable-Declaration-Pr>    | $                                 
<Variable-Declaration-Part>  | identifier

<Constant-Declaration>       | const                             FOLLOW(<Declaration-Part>)
<Constant-Declaration-Pr>    | ;
<Constant-Declaration-Pr>    | $                                 FOLLOW(<Constant-Declaration>) 
<Constant-Declaration-Part>  | identifier

<Statement-Part>             | begin

<Identifier-List>            | identifier                        :
<Identifier-List-Pr>         | ,                                 FOLLOW(<Identifier-List>)
<Identifier-List-Pr>         | $

## Procedures

<Procedure-Declaration>      | procedure
<Parameter-List>             | identifier
<Parameter-List-Pr>          | ,                                 FOLLOW(<Parameter-List>)
<Parameter-List-Pr>          | $                                 
<Parameter>                  | identifier
<Return-Type>                | :
<Return-Type>                | $

## Statements

<Statement-Sequence>         | if, for, while, begin, identifier
<Statement-Sequence-Pr>      | ;
<Statement-Sequence-Pr>      | $ 

<Statement>                  | if 
<Statement>                  | for, while
<Statement>                  | begin
<Statement>                  | identifier

<Conditional-Statement>      | if
<If-Statement>               | if
<If-Statement-Pr>            | else
<If-Statement-Pr>            | $

<Repeditive-Statement>       | while
<Repeditive-Statement>       | for
<While-Statement>            | while
<For-Statement>              | for 
<For-Statement-Pr>           | to 
<For-Statement-Pr>           | downto 
<For-Statement-End>          | do 

<Block-Statement>            | begin

<Regular-Statement>          | identifier
<Regular-Statment-Pr>        | :
<Regular-Statement-Pr>       | (
<Assignment-Statement>       | :
<Procedure-Statement>        | (
<Usage-Parameter-List>       | identifier, number, ( 
<Usage-Parameter-List-Pr>    | ,
<Usage-Parameter-List-Pr>    | $
<Usage-Parameter>            | identifier, number, (

## Expressions

<Expression>                 | identifier, number, (
<Expression-Pr>              | =, <, >, <=, >=, <>
<Expression-Pr>              | $ 

<Comparison-Operator>        | = 
<Comparison-Operator>        | <
<Comparison-Operator>        | >
<Comparison-Operator>        | <=
<Comparison-Operator>        | >=
<Comparison-Operator>        | <>

<Base-Expression>            | identifier, number, (
<Base-Expression-Pr>         | +, -, or 
<Base-Expression-Pr>         | $ 

<Plus-Minus-Or>              | + 
<Plus-Minus-Or>              | -
<Plus-Minus-Or>              | or

<Term>                       | identifier, number, (
<Term-Pr>                    | * , /, and
<Term-Pr>                    | $

<Mult-Div-And>               | *
<Mult-Div-And>               | /
<Mult-Div-And>               | and

<Factor>                     | identifier 
<Factor>                     | number 
<Factor>                     | (

## Others

<Type>                       | integer 
<Type>                       | array 

<Array-Type>                 | array 






