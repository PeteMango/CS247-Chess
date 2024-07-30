#!/bin/bash

EXECUTABLE="./chess"
MAX_MOVES=5000

levels=("computer1" "computer2" "computer3" "computer4", "computer5")

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
			# sleep 0.5
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

if [[ $# -eq 2 ]]; then
	run_game "$1" "$2"
elif [[ $# -eq 0 ]]; then
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
