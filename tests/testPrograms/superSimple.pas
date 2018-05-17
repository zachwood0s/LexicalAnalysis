program supersimple;

procedure test(n: integer); forward;
procedure factorization(n: integer);
var I, TEMP, NUM, SUM : integer;
begin
    if n = 1 then 
    begin
        factorization := 1;
        I := 5;
    end
    else
        factorization := n * factorization(n-1);

    test(I);
end;

procedure test(n: integer);
begin
    writeln(n);
        {exit;}
end;

var J:integer;

begin
    J:=3;
    writeln(factorization(factorization(J)));
end.

{
    So else statements are not working right now. that needs to be looked at
    also make while loops work
    also make sure everything is actually working correctly (obviously)
}
