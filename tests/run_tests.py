import unittest
import subprocess
import os


def RunTest():
    in_dir = "in"
    out_dir = "out"
    binary = "./chess"

    passed = 0
    failed = 0

    for root, _, files in os.walk(in_dir):
        for file in files:
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
                        result = subprocess.run(
                            [binary],
                            stdin=infile,
                            text=True,
                            capture_output=True,
                            check=True,
                        )
                    with open(output_file, "w") as outfile:
                        outfile.write(result.stdout)
                    passed += 1
                except subprocess.CalledProcessError as e:
                    failed += 1
                    print(f"Error running {binary} with input file {input_file}: {e}")
                except Exception as e:
                    print(f"An unexpected error occurred: {e}")

    print("\n\n")
    print(f"PASSED: {passed}")
    print(f"FAILED: {failed}")
    print("\n\n")


if __name__ == "__main__":
    RunTest()
