#!/bin/bash

make clean

make

if [ "$1" == "--valgrind" ]; then
	./tests/run_tests.sh --valgrind --multigame
else
	./tests/run_tests.sh
fi
