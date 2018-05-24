program doubleRecursionLoop;

procedure rec2(n : integer); forward;
procedure rec1(n : integer);
begin
    writeln(n);
    if n = 0 then exit;
    rec2(n);
    rec1(n-1);
end;

procedure rec2(n : integer);
begin
    writeln(n);
    if n = 0 then exit;
    rec2(n-1);
end;

begin
    rec1(5);
end.

