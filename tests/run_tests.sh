#!/bin/bash

BASE_DIR="tests"
OUT_DIR="$BASE_DIR/out"
IN_DIR="$BASE_DIR/in/valid"
EXP_DIR="$BASE_DIR/expect/valid"
EXECUTABLE="./out/chess"
FLAGS="--no-graphics"

total=0
passed=0
crash=0
valgrind_check=0

mkdir -p "$OUT_DIR"

declare -a crashed
declare -a diffs
declare -a valgrind_passed
declare -a valgrind_failed

run_test() {
	local INPUT_DIR="$1"
	local EXPECTED_DIR="$2"
	local TEST_TYPE="$3"
	local USE_VALGRIND="$4"

	OUTPUT_DIR="$OUT_DIR/out/$TEST_TYPE"
	DIFF_DIR="$OUT_DIR/diff/$TEST_TYPE"
	VALGRIND_DIR="$OUT_DIR/valgrind/$TEST_TYPE"

	mkdir -p "$OUTPUT_DIR"
	mkdir -p "$DIFF_DIR"
	mkdir -p "$VALGRIND_DIR"

	for input_file in "$INPUT_DIR"/*; do
		filename=$(basename "$input_file" .in)

		output_file="$OUTPUT_DIR/$filename.out"
		expected_file="$EXPECTED_DIR/$filename.expect"
		diff_file="$DIFF_DIR/$filename.diff"
		valgrind_file="$VALGRIND_DIR/$filename.valgrind"
		if [ "$USE_VALGRIND" -eq 1 ]; then
			echo "Processing $filename with valgrind..."
			if ! valgrind --leak-check=full "$EXECUTABLE" $FLAGS <"$input_file" >"$output_file" 2>"$valgrind_file"; then
				echo "Executable CRASHED on $filename"
				crashed+=("$filename")
				((crash++))
				exit 1
				# continue
			fi

			if grep -q "All heap blocks were freed -- no leaks are possible" "$valgrind_file"; then
				echo "Valgrind test $filename passed"
				valgrind_passed+=("$filename")
			else
				echo "Valgrind test $filename FAILED"
				valgrind_failed+=("$filename")
				exit 1
			fi
		else
			echo "Processing $filename without valgrind..."
			if ! "$EXECUTABLE" $FLAGS <"$input_file" >"$output_file"; then
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
		fi

		((total++))
	done
}

while [ $# -gt 0 ]; do
	case $1 in
	--valgrind)
		valgrind_check=1
		shift
		;;
	--checkmate)
		run_test "$IN_DIR/checkmate" "$EXP_DIR/checkmate" "checkmate" "$valgrind_check"
		shift
		;;
	--stalemate)
		run_test "$IN_DIR/stalemate" "$EXP_DIR/stalemate" "stalemate" "$valgrind_check"
		shift
		;;
	--resign)
		run_test "$IN_DIR/resign" "$EXP_DIR/resign" "resign" "$valgrind_check"
		shift
		;;
	--multigame)
		run_test "$IN_DIR/multigame" "$EXP_DIR/multigame" "multigame" "$valgrind_check"
		shift
		;;
	--all)
		run_test "$IN_DIR/checkmate" "$EXP_DIR/checkmate" "checkmate" "$valgrind_check"
		run_test "$IN_DIR/stalemate" "$EXP_DIR/stalemate" "stalemate" "$valgrind_check"
		run_test "$IN_DIR/resign" "$EXP_DIR/resign" "resign" "$valgrind_check"
		run_test "$IN_DIR/multigame" "$EXP_DIR/multigame" "multigame" "$valgrind_check"
		shift
		;;
	*)
		echo "Invalid option: $1" >&2
		exit 1
		;;
	esac
done

if [ $total -eq 0 ]; then
	run_test "$IN_DIR/checkmate" "$EXP_DIR/checkmate" "checkmate" "$valgrind_check"
	run_test "$IN_DIR/stalemate" "$EXP_DIR/stalemate" "stalemate" "$valgrind_check"
	run_test "$IN_DIR/resign" "$EXP_DIR/resign" "resign" "$valgrind_check"
	run_test "$IN_DIR/multigame" "$EXP_DIR/multigame" "multigame" "$valgrind_check"
fi

echo "Total tests: $total"
echo "Passed tests: $passed"
echo "Crashes: $crash"
echo "Differences found in the following files:"
printf "%s\n" "${diffs[@]}"

echo "Crashed on the following files:"
printf "%s\n" "${crashed[@]}"
