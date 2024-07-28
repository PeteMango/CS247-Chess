#!/bin/bash

make

if [ $? -eq 0 ]; then
    ./test_moves
else
    echo "Compilation failed"
fi
