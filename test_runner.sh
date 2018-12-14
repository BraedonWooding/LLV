# Our Testing Script
# Runs the collection tests as well as the output tests
# Collection tests are orientated around just checking that the collections work as expected
# Output tests just check the program hasn't regressed and test all the examples and more!

if [ -z $CC ]; then CC=clang; fi

echo "== Testing Collections =="
for test in collection_tests/*.out; do
    echo "Running $test"
    # We can't just include it like `clang libLLV.a my_file.c` like in clang
    # we have to do it this way because GCC is ancient in some regards and this
    # is one of them.
    ./$test
    if [ $? -ne 0 ]; then
        echo "Testing Collections Failed"
        echo "ERROR: Test $test failed"
        exit 1
    fi
done

printf "\n== Testing Collections Passed ==\n"
printf "\n== Testing Output ==\n"

for test in output_tests/*.out; do
    printf "\nRunning $test\n"
    filename=${test%.*}
    for test_case in test_matrix/*; do
        source $test_case
        ./$test > $filename.result
        test_case_name=$(basename $test_case)
        if [ ! -f $filename.expected."${test_case_name%%.*}" ]; then
            echo "Missing $filename.expected."${test_case_name%%.*}" exiting"
            exit 2
        fi
        diff $filename.result $filename.expected."${test_case_name%%.*}"
        if [ $? -ne 0 ]; then
            echo "Testing Output Failed"
            echo "ERROR: Test $test:$test_case failed difference shown above exiting"
            exit 1
        fi
        printf "\n$test:$test_case successful\n"
    done
    printf "\n$test successful\n"
done

printf "\n== Testing Output Passed ==\n"
printf "\nCleaning up tmp files\n"

rm collection_tests/*.out
rm output_tests/*.out
rm output_tests/*.result
