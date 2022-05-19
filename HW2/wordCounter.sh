#!/bin/bash

number=$1

shift 

echo "Number of files: $#"
  
files=$@

echo "Processing: $files"

for file in $files
do 

line=$(wc -l < $file)
words=$(wc -w < $file)
char=$(wc -m < $file)

echo "===($line $words $char $file)==="
grep -oE '[[:alnum:]]+' $file | sort | uniq -c | sort -nr | head -$number


done

echo "==done==="
