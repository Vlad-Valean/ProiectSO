#!/bin/bash

if cat "$2" | grep -Eq '(corrupted|dangerous|risk|attack|malware|malicious)'
then
    echo "$2"
    mv "$2" "$1"
fi

