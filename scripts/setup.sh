#!/bin/bash

for i in {1..9}
do
    if [ "$1" = "c" ]; then
        mkdir "../dir$i"
        mkdir "../dir$i/subdir1"
        mkdir "../dir$i/subdir2"
        mkdir "../dir$i/subdir2/subdir1"
        echo "ceva1" > "../dir$i/file.txt"
        echo "ceva1" > "../dir$i/subdir1/file.txt"
        echo "ceva2" > "../dir$i/subdir2/subdir1/file1.txt"
        echo "ceva3" > "../dir$i/subdir2/file.txt"
        echo "ceva4" > "../dir$i/subdir2/subdir1/file2.txt"
    elif [ "$1" = "r" ]; then
        rm -rf "../dir$i"
        rm -rf "../target"
        rm -rf "../quarantine"
    else 
        echo "./setup.sh c (create) / r (remove)";
        exit 1
    fi
done