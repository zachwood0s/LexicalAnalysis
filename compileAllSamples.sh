search_dir=./tests/testPrograms/samples
for entry in "$search_dir"/*
do
  echo "Compiling $entry"
  base=$(basename $entry)
  bitcode="$search_dir"/OutBitcode/"$base"
  object="$search_dir"/OutObjects/"$base"
  execute="$search_dir"/OutExecutables/"$base"
  ./compiler $entry $bitcode 
  llc -relocation-model=pic -filetype=obj "$bitcode".bc -o "$object".o
  gcc "$object".o -o $execute
  #echo "$search_dir"/OutBitcode/"$base"
done

