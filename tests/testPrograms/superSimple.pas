program supersimple;

function factor(n: integer): integer;
var I, J: integer;
const TEST = 5;
begin
    while n <= 5 do
    begin
        writeln(n);
        readln(I);
        if I > TEST then break;
        n := n + 1;
    end;
    TEST := 12;
    writeln(n);
    {if n < 4 then
    begin
        writeln(n);
        if n < 3 then exit;
        writeln(n-3);
    end
    else 
        writeln(n+1);

    for I:= 10 downto 0 do 
    begin
        if I = 4 then break
        else if I = 5 then writeln(I + 10)
        else if I > 7 then writeln(I - 10)
        else 
            for J:=0 to 4 do 
            begin
                if J = 3 then break;
                writeln(J);
            end;
    end;
    writeln(20);}
end;

var N: integer;

begin
    readln(N);
    factor(N);
end.

{
    So else statements are not working right now. that needs to be looked at
    also make while loops work
    also make sure everything is actually working correctly (obviously)
}
