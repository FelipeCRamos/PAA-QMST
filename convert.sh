#!/bin/bash

for file in $(ls tests/10-30/)
do
    echo "$file"
    python3 src/parser.py tests/10-30/$file
    echo -e "\tDone"
done

for file in $(ls tests/10-50/)
do
    echo "$file"
    python3 src/parser.py tests/10-50/$file
    echo -e "\tDone"
done
