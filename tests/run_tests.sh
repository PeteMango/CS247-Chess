#!/bin/bash

BASE_DIR="tests"
OUT_DIR="$BASE_DIR/out"
IN_DIR="$BASE_DIR/in/valid"
EXP_DIR="$BASE_DIR/expect/valid"
EXECUTABLE="out/chess"

total=0
passed=0
crash=0

mkdir -p "$OUT_DIR"

declare -a crashed
declare -a diffs

run_test() {
	local INPUT_DIR="$1"
	local EXPECTED_DIR="$2"
	local TEST_TYPE="$3"

	OUTPUT_DIR="$OUT_DIR/out/$TEST_TYPE"
	DIFF_DIR="$OUT_DIR/diff/$TEST_TYPE"

	mkdir -p "$OUTPUT_DIR"
	mkdir -p "$DIFF_DIR"

	for input_file in "$INPUT_DIR"/*; do
		filename=$(basename "$input_file" .in)
		echo "Processing $filename..."

		output_file="$OUTPUT_DIR/$filename.out"
		expected_file="$EXPECTED_DIR/$filename.expect"
		diff_file="$DIFF_DIR/$filename.diff"

		if ! "$EXECUTABLE" <"$input_file" >"$output_file"; then
			echo "Executable CRASHED on $filename"
			crashed+=("$filename")
			((crash++))
			exit 1
			# continue
		fi

		if diff "$expected_file" "$output_file" >"$diff_file"; then
			echo "Test $filename passed"
			((passed++))
		else
			echo "Test $filename FAILED, differences found"
			diffs+=("$filename")
			exit 1
		fi

		((total++))
	done
}

while [ $# -gt 0 ]; do
	case $1 in
	--checkmate)
		run_test "$IN_DIR/checkmate" "$EXP_DIR/checkmate" "checkmate"
		shift
		;;
	--stalemate)
		run_test "$IN_DIR/stalemate" "$EXP_DIR/stalemate" "stalemate"
		shift
		;;
	--resign)
		run_test "$IN_DIR/resign" "$EXP_DIR/resign" "resign"
		shift
		;;
	--multigame)
		run_test "$IN_DIR/multigame" "$EXP_DIR/multigame" "multigame"
		shift
		;;
	--all)
		run_test "$IN_DIR/checkmate" "$EXP_DIR/checkmate" "checkmate"
		run_test "$IN_DIR/stalemate" "$EXP_DIR/stalemate" "stalemate"
		run_test "$IN_DIR/resign" "$EXP_DIR/resign" "resign"
		run_test "$IN_DIR/multigame" "$EXP_DIR/multigame" "multigame"
		shift
		;;
	*)
		echo "Invalid option: $1" >&2
		exit 1
		;;
	esac
done

if [ $total -eq 0 ]; then
	run_test "$IN_DIR/checkmate" "$EXP_DIR/checkmate" "checkmate"
	run_test "$IN_DIR/stalemate" "$EXP_DIR/stalemate" "stalemate"
	run_test "$IN_DIR/resign" "$EXP_DIR/resign" "resign"
	run_test "$IN_DIR/multigame" "$EXP_DIR/multigame" "multigame"
fi

echo "Total tests: $total"
echo "Passed tests: $passed"
echo "Crashes: $crash"
echo "Differences found in the following files:"
printf "%s\n" "${diffs[@]}"

echo "Crashed on the following files:"
printf "%s\n" "${crashed[@]}"
