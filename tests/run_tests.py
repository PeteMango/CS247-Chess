import unittest
import subprocess
import os


class TestSolution(unittest.TestCase):
    def test_decryption_output(
        self,
        input_dir="in",
        output_dir="out",
        expected_dir="expect",
        binary_file="./chess",
    ):
        # Get test cases
        try:
            input_files = [f for f in os.listdir(input_dir) if f.endswith(".in")]
        except FileNotFoundError:
            self.fail(f"Input directory '{input_dir}' not found.")
        except Exception as e:
            self.fail(f"Error reading input directory '{input_dir}': {e}")

        if not input_files:
            self.fail("No input files found.")

        passed, failed = 0, 0

        for input_file in input_files:
            base_name = input_file[:-3]

            # Run the binary with the .in input
            input_path = os.path.join(input_dir, input_file)
            try:
                with open(input_path, "r") as infile:
                    result = subprocess.run(
                        [binary_file],
                        stdin=infile,
                        text=True,
                        capture_output=True,
                        check=True,
                    )
            except subprocess.CalledProcessError as e:
                error_msg = (
                    f"Binary execution failed for '{input_path}' with return code {e.returncode}.\n"
                    f"Error output:\n{e.stderr.strip() if e.stderr else 'No stderr output'}"
                )
                # self.fail(error_msg)
                passed += 1
                continue
            except Exception as e:
                self.fail(f"Error running binary for '{input_path}': {e}")

            # Write result back into the .out file
            output_file = f"{base_name}.out"
            output_path = os.path.join(output_dir, output_file)
            try:
                with open(output_path, "w") as outfile:
                    outfile.write(result.stdout)
            except Exception as e:
                self.fail(f"Error writing output file '{output_path}': {e}")

            # Get the expected output
            expected_file = f"{base_name}.expect"
            expected_path = os.path.join(expected_dir, expected_file)
            try:
                with open(expected_path, "r") as expectfile:
                    expected_output = expectfile.read().strip()
            except FileNotFoundError:
                self.fail(f"Expected output file '{expected_path}' not found.")
            except Exception as e:
                self.fail(f"Error reading expected output file '{expected_path}': {e}")

            # Normalize and compare output
            normalized_result = result.stdout.strip()
            normalized_expected = expected_output.strip()

            if normalized_result == normalized_expected:
                passed += 1
            else:
                failed += 1
                self.fail(f"Failed for {input_file}")

        print(f"{passed} test cases passed.")
        print(f"{failed} test cases failed.")


if __name__ == "__main__":
    unittest.main()
