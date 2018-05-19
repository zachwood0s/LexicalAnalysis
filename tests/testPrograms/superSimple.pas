program supersimple;

function factor(n: integer): integer;
var I, J: integer;
begin
    if n < 4 then
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
    writeln(20);
end;

begin
    factor(3);
end.

{
    So else statements are not working right now. that needs to be looked at
    also make while loops work
    also make sure everything is actually working correctly (obviously)
}
