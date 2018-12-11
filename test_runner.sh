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
    ./$test > $filename.result
    diff $filename.result $filename.expected
    if [ $? -ne 0 ]; then
        echo "Testing Output Failed"
        echo "ERROR: Test $test failed difference shown above"
        exit 1
    fi
    printf "\n$test successful\n"
done

printf "\n== Testing Output Passed ==\n"
printf "\nCleaning up tmp files\n"

rm tmp.out
rm output_tests/*.out
