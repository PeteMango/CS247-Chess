#!/bin/bash

EXECUTABLE="./out/chess"
MAX_MOVES=1000

# Define the available levels
# levels=("computer1" "computer2" "computer3" "computer4")
levels=("computer1", "computer2")

# Function to run the game with provided flags
run_game() {
	local flag1=$1
	local flag2=$2
	local move_count=0
	local output
	local final_score_found=0

	{
		echo "game $flag1 $flag2"
		while [[ $final_score_found -eq 0 && $move_count -lt $MAX_MOVES ]]; do
			echo "move"
			((move_count++))
		done
	} | $EXECUTABLE | while IFS= read -r line; do
		echo "$line"
		if [[ "$line" == *"Final Score"* ]] || [[ "$line" == *"Checkmate"* ]] || [[ "$line" == *"Stalemate"* ]]; then
			final_score_found=1
			break
		fi
	done

}

# Check the number of arguments
if [[ $# -eq 2 ]]; then
	# Run game with the provided flags
	run_game "$1" "$2"
elif [[ $# -eq 0 ]]; then
	# Run game with every permutation of the flags
	echo "Running with all permutations"
	for flag1 in "${levels[@]}"; do
		for flag2 in "${levels[@]}"; do
			run_game "$flag1" "$flag2"
		done
	done
else
	echo "Invalid number of arguments. Please provide either 2 flags or 0 flags."
	exit 1
fi
