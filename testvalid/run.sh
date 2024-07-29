#!/bin/bash

make

if [ $? -eq 0 ]; then
    ./testmoves
else
    echo "Compilation failed"
fi
