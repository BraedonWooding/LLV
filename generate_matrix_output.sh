# PLEASE only use this on a test case once you know
# that the test case is producing correct output
# else you will just cause false positives to pop up

cmake .
make

for test in test_matrix/*; do
    source $test
    file=$(basename $test)
    for f in $@; do
        $f > "${f%%.*}".expected."${file%%.*}"
    done
done
