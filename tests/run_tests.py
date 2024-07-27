import unittest
import subprocess
import os
import io


def RunTest():
    in_dir = "in"
    out_dir = "out"
    binary = "./chess"

    total = 0
    passed = 0
    crash = 0

    crashed = []
    fail_files_count = 0

    for root, _, files in os.walk(in_dir):
        for file in files:
            if file.startswith("fail"):
                fail_files_count += 1
            if file.endswith("moving.in"):
                continue

            if file.endswith(".in"):
                rel_path = os.path.relpath(root, in_dir)
                input_file = os.path.join(root, file)
                output_subdir = os.path.join(out_dir, rel_path)
                if not os.path.exists(output_subdir):
                    os.makedirs(output_subdir)

                name = file[:-3]
                output_file = os.path.join(output_subdir, f"{name}.out")

                try:
                    with open(input_file, "r") as infile:
                        # Read lines and look for the exclamation mark
                        lines = infile.readlines()
                        start_index = 0
                        pre_exclamation = []

                        for i, line in enumerate(lines):
                            if line.strip() == "!":
                                start_index = i + 1
                                break
                            pre_exclamation.append(line)

                        # If exclamation mark is found, start reading from the next line
                        # Otherwise, start reading from the beginning
                        relevant_lines = lines[start_index:]

                        # Print the part before the exclamation mark if it exists
                        if pre_exclamation:
                            print(f"Content before exclamation mark in {input_file}:")
                            print("".join(pre_exclamation))

                        # Create a string from relevant lines to use as input
                        input_data = "".join(relevant_lines)

                        # Run the subprocess with the relevant lines as input
                        result = subprocess.run(
                            [binary],
                            input=input_data,
                            text=True,
                            capture_output=True,
                            check=True,
                        )

                    with open(output_file, "w") as outfile:
                        passed += 1
                        outfile.write(result.stdout)

                except subprocess.CalledProcessError as e:
                    crash += 1
                    crashed.append((input_file, str(e)))
                    print(f"Error running {binary} with input file {input_file}: {e}")
                except Exception as e:
                    crash += 1
                    crashed.append((input_file, str(e)))
                    print(f"An unexpected error occurred: {e}")
            total += 1

    print("\n\n")
    print(f"TOTAL: {total}")
    print(f"PASSED: {passed}")
    print(f"CRASHED: {crash}")

    print("\n\nCRASHED TEST CASES:")
    for crash_file, error in crashed:
        print(f"File: {crash_file}")
        with open(crash_file, "r") as file:
            print(file.read())
        print(f"Error: {error}")
        print("\n")

    # Check if the number of crashes is equal to the number of files that start with "fail"
    if crash == fail_files_count:
        print("TESTCASES PASS")
    else:
        raise AssertionError(
            f"Discrepancy found: {crash} crashes vs. {fail_files_count} 'fail' files."
        )


if __name__ == "__main__":
    RunTest()
