#!/bin/bash

make

if [ $? -eq 0 ]; then
    ./validmoves
else
    echo "Compilation failed"
fi
