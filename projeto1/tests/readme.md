# Łukasiewicz tests

* Run lukacompiler passing an input using "<" and saving its output using ">". Example: ./lukacompiler < tests/v0.1/input/1.in > 1.out
* Compare the output with the expected output using diff. Example: diff 1.out tests/v0.1/expected/1.ex
* If any differences are found, your compiler is not respecting the project specification
* To run all tests from determinated version or all tests from all versions use tests.sh script. Examples: "./tests.sh 0.1" or "./tests.sh all"
