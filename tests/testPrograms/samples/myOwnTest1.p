program myTest1;

function getUserInt() : integer;
var in : integer;
begin
    readln(in);
    getUserInt := in;
end;

begin
    writeln(getUserInt());
end.
