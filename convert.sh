#!/bin/bash

# Use para converter os arquivos baixados do http://homes.di.unimi.it/cordone/research/qmst.html

folder1=tests/10-30
folder2=tests/10-50

for file in $(ls $folder1)
do
    echo "$file"
    python3 src/parser.py $folder1/$file
    echo -e "\tDone"
done

for file in $(ls $folder2)
do
    echo "$file"
    python3 src/parser.py $folder2/$file
    echo -e "\tDone"
done
