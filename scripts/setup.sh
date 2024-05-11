#!/bin/bash

for i in {1..9}
do
    if [ "$1" = "c" ]; then
        mkdir "../dir$i"
        chmod 777 "../dir$i"
        
        mkdir "../dir$i/subdir1"
        chmod 777 "../dir$i/subdir1"
        
        mkdir "../dir$i/subdir2"
        chmod 777 "../dir$i/subdir2"
        
        mkdir "../dir$i/subdir2/subdir1"
        chmod 777 "../dir$i/subdir2/subdir1"
        
        openssl rand -base64 500 > "../dir$i/file.txt"
        echo " corrupted" >> "../dir$i/file.txt"
        openssl rand -base64 100 >> "../dir$i/file.txt"
        chmod 642 "../dir$i/file.txt"
        
        openssl rand -base64 500 > "../dir$i/subdir1/file.txt"
        chmod 777 "../dir$i/subdir1/file.txt"
        
        openssl rand -base64 500 > "../dir$i/subdir2/subdir1/file1.txt"
        echo " corrupted" >> "../dir$i/subdir2/subdir1/file1.txt"
        openssl rand -base64 100 >> "../dir$i/subdir2/subdir1/file1.txt"
        chmod 777 "../dir$i/subdir2/subdir1/file1.txt"

        openssl rand -base64 500 > "../dir$i/subdir2/file.txt"
        chmod 777 "../dir$i/subdir2/file.txt"
        
        openssl rand -base64 500 > "../dir$i/subdir2/subdir1/file2.txt"
        chmod 777 "../dir$i/subdir2/subdir1/file2.txt"

    elif [ "$1" = "r" ]; then
        rm -rf "../dir$i"
        rm -rf "../target"
        rm -rf "../quarantine"
    else 
        echo "./setup.sh c (create) / r (remove)";
        exit 1
    fi
done