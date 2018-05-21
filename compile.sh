echo "Compiling $1"
./compiler $1 $1
name=$(echo "$1" | cut -f 1 -d '.')
llc -relocation-model=pic -filetype=obj "$1".bc -o "$1".o
gcc "$1".o -o "$name"
