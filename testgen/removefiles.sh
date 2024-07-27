#!/bin/bash

# Find and delete all .in files
find . -type f -name "*.in" -exec rm -f {} \;

# Find and delete all .expect files
find . -type f -name "*.expect" -exec rm -f {} \;

echo "All .in and .expect files have been removed."
