#!/bin/bash

make clean

make

./tests/run_tests.sh --no-graphics
