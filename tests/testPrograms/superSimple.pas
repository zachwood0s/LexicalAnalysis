program test;

var I, J:integer;
begin
    while I < 10 do
    begin
        writeln(I);
        inc(I);
    end;
    while I > 0 do
    begin
        J := 0;
        while J < 3 do
        begin
            writeln(J);
            inc(J);
        end;
        writeln(I);
        dec(I);
    end;

end.
