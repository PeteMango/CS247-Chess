#!/bin/bash

# Set variables
# checkmate
# EXPECTED_DIR="tests/expect/valid/checkmate"
# INPUT_DIR="tests/in/valid/checkmate"
# stalemate
EXPECTED_DIR="tests/expect/valid/stalemate"
INPUT_DIR="tests/in/valid/stalemate"
OUTPUT_DIR="tests/norman/out"
DIFF_DIR="tests/norman/diff"
EXECUTABLE="out/chess"

total=0
passed=0
crash=0

# Create output and diff directories if they do not exist
mkdir -p "$OUTPUT_DIR"
mkdir -p "$DIFF_DIR"

declare -a crashed
declare -a diffs
# Iterate through all files in the input directory
for input_file in "$INPUT_DIR"/*; do
	# Extract the filename without the path
	filename=$(basename "$input_file" .in)
	echo "Processing $filename..."

	# Define output and expected file paths
	output_file="$OUTPUT_DIR/$filename.out"
	expected_file="$EXPECTED_DIR/$filename.expect"
	diff_file="$DIFF_DIR/$filename.diff"

	# Run the executable with the input file and redirect output
	if ! "$EXECUTABLE" <"$input_file" >"$output_file"; then
		echo "Executable CRASHED on $filename"
		crashed+=("$filename")
		((crash++))
		continue
	fi

	# Diff the expected output and actual output
	if diff "$expected_file" "$output_file" >"$diff_file"; then
		echo "Test $filename passed"
		((passed++))
	else
		echo "Test $filename FAILED, differences found"
		diffs+=("$filename")
	fi

	((total++))
done

echo "Total tests: $total"
echo "Passed tests: $passed"
echo "Crashes: $crash"
echo "Differences found in the following files:"
printf "%s\n" "${diffs[@]}"

echo "Crashed on the following files:"
printf "%s\n" "${crashed[@]}"
