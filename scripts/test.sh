#!/bin/bash

make clean

make 

rm tests/chess

cp out/chess tests/

cd tests/

python3 run_tests.py