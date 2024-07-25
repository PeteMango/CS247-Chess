import unittest
import subprocess
import os

class TestSolution(unittest.TestCase):
    def test_decryption_output(self, input_dir='in', output_dir='out', expected_dir='expect', binary_file='./chess'):
        # get test cases
        tc = []
        for f in os.listdir(input_dir):
            if f.endswith('.in'):
                tc.append(f)
        
        for input_file in tc:
            # trim the .in part
            base_name = input_file[:-3]
            
            # run the binary with the .in input
            input_path = os.path.join(input_dir, input_file)
            with open(input_path, 'r') as infile:
                result = subprocess.run([binary_file], stdin=infile, text=True, capture_output=True)
            
            # write result back into the .out file
            output_file = f'{base_name}.out'
            output_path = os.path.join(output_dir, output_file)
            with open(output_path, 'w') as outfile:
                outfile.write(result.stdout)
            
            # get the expected output 
            expected_file = f'{base_name}.expect'
            expected_path = os.path.join(expected_dir, expected_file)
            with open(expected_path, 'r') as expectfile:
                expected_output = expectfile.read().strip()
            
            # normalize and compare output
            normalized_result = result.stdout.strip()
            normalized_expected = expected_output.strip()

            # print(f"Length of result.stdout: {len(normalized_result)}")
            # print(f"Length of expected_output: {len(normalized_expected)}")
            # print(f"repr(result.stdout): {repr(normalized_result)}")
            # print(f"repr(expected_output): {repr(normalized_expected)}")

            self.assertEqual(normalized_result, normalized_expected, f"Failed for {input_file}")

if __name__ == '__main__':
    unittest.main()
