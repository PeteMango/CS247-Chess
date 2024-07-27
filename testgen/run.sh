#!/bin/bash

g++ -std=c++17 -O3 -o create_tests main.cpp -lcurl

if [ $? -eq 0 ]; then
    ./create_tests
else
    echo "Compilation failed"
fi
