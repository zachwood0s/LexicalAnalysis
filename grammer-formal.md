
# Formal Grammar

## Non-Terminals are written in <name>

## Main Program

<Program>                   => <Program-Header><Block>                                          
<Program-Header>            => program identifier                                               

<Block>                     => <Declaration-Part><Statement-Part>                               
<Declaration-Part>          => <Variable-Declaration>                                           
<Declaration-Part>          => <Constant-Declaration>                                           
<Declaration-Part>          => <Procedure-Declaration>                                          
<Declaration-Part>          => $

<Variable-Declaration>      => var <Variable-Declaration-Part><Variable-Declaration-Pr>
<Variable-Declaration-Pr>   => ;<Variable-Declaration-Part><Variable-Declaration-Pr>
<Variable-Declaration-Pr>   => $
<Variable-Declaration-Part> => <Identifier-List>:<Type>

<Constant-Declaration>      => const <Contant-Declaration-Part><Constant-Declaration-Pr>
<Constant-Declaration-Pr>   => ;<Constant-Declaration-Part><Constant-Declaration-Pr>
<Constant-Declaration-Pr>   => $
<Constant-Declaration-Part> => identifier = constant

<Statement-Part>            => begin <Statement-Sequence> end

<Identifier-List>           => identifier<Identifier-List-Pr>
<Identifier-List-Pr>        => ,identifier<Identifier-List-Pr>
<Identifier-List-Pr>        => $

## Procedures

<Procedure-Declaration>     => procedure identifier <Parameter-List><Return-Type>;
<Parameter-List>            => <Parameter><Parameter-List-Pr>
<Parameter-List-Pr>         => ,<Parameter><Parameter-List-Pr>
<Parameter-List-Pr>         => $
<Parameter>                 => identifier:<Type>
<Return-Type>               => :<Type>
<Return-Type>               => $

## Statements

<Statement-Sequence>        => <Statement><Statement-Sequence-Pr>
<Statement-Sequence-Pr>     => ;<Statement><Statement-Sequence-Pr>
<Statement-Sequence-Pr>     => $

<Statement>                 => <Conditional-Statement>
<Statement>                 => <Repeditive-Statement>
<Statement>                 => <Block-Statement>
<Statement>                 => <Regular-Statement>

<Conditional-Statement>     => <If-Statement>
<If-Statement>              => if <Expression> then <Statement> <If-Statement-Pr>
<If-Statement-Pr>           => else <Statement>
<If-Statement-Pr>           => $

<Repeditive-Statement>      => <While-Statement>
<Repeditive-Statement>      => <For-Statement>
<While-Statement>           => while <Expression> do <Statement>
<For-Statement>             => for identifier := <Expression> <For-Statement-Pr>      
<For-Statement-Pr>          => to <Expression> <For-Statement-End>
<For-Statement-Pr>          => downto <For-Statement-End>
<For-Statement-End>         => do <Statement>

<Block-Statement>           => begin <Statement-Sequence> end

<Regular-Statement>         => identifier<Regular-Statemnt-Pr>
<Regular-Statement-Pr>      => <Assignment-Statement>
<Regular-Statement-Pr>      => <Procedure-Statement>
<Assignment-Statement>      => := <Expression>
<Procedure-Statement>       => (<Usage-Parameter-List>)
<Usage-Parameter-List>      => <Usage-Parameter><Usage-Parameter-List-Pr>
<Usage-Parameter-List-Pr>   => ,<Usage-Parameter><Usage-Parameter-List-Pr>
<Usage-Parameter-List-Pr>   => $
<Usage-Parameter>           => <Expression>

## Expressions

<Expression>                => <Base-Expression><Expression-Pr>
<Expression-Pr>             => <Comparison-Operator><Base-Expression><Expression-Pr>
<Expression-Pr>             => $

<Comparison-Operator>       => =
<Comparison-Operator>       => <
<Comparison-Operator>       => >
<Comparison-Operator>       => <=
<Comparison-Operator>       => >=
<Comparison-Operator>       => <>

<Base-Expression>           => <Term><Base-Expression-Pr>
<Base-Expression-Pr>        => <Plus-Minus-Or><Term><Base-Expression-Pr>
<Base-Expression-Pr>        => $

<Plus-Minus-Or>             => +
<Plus-Minus-Or>             => -
<Plus-Minus-Or>             => or

<Term>                      => <Factor><Term-Pr>
<Term-Pr>                   => <Mult-Div-And><Factor><Term-Pr>
<Term-Pr>                   => $

<Mult-Div-And>              => *
<Mult-Div-And>              => /
<Mult-Div-And>              => and

<Factor>                    => identifier
<Factor>                    => number
<Factor>                    => (<Expression>)

## Others

<Type>                      => integer
<Type>                      => <Array-Type>

<Array-Type>                => array [<Expression> .. <Expression>] of <Type>






