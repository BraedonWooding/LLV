# Our Testing Script
# Runs the collection tests as well as the output tests
# Collection tests are orientated around just checking that the collections work as expected
# Output tests just check the program hasn't regressed and test all the examples and more!

cmake -DCMAKE_BUILD_TYPE=Debug .
make

echo "== Testing Collections =="
for test in collection_tests/*.c; do
    echo "Running $test"
    gcc libLLV.a $test -o tmp.out
    ./tmp.out
    if [ $? -ne 0 ]; then
        echo "Testing Collections Failed"
        echo "ERROR: Test $test failed"
        exit 1
    fi
done

echo "\n== Testing Collections Passed =="
echo "\n== Testing Output =="

for test in output_tests/*.c; do
    echo "\nRunning $test"
    filename=${test%.*}
    gcc libLLV.a $test -o tmp.out
    ./tmp.out > $filename.out
    diff $filename.out $filename.expected
    if [ $? -ne 0 ]; then
        echo "Testing Output Failed"
        echo "ERROR: Test $test failed difference shown above"
        exit 1
    fi
    echo "\n$test successful"
done

echo "\n== Testing Output Passed =="
echo "\nCleaning up tmp files"

rm tmp.out
rm output_tests/*.out
