# Our Testing Script
# Runs the collection tests as well as the output tests
# Collection tests are orientated around just checking that the collections work as expected
# Output tests just check the program hasn't regressed and test all the examples and more!

if [ -z $CC ]; then CC=clang; fi

GREEN="\e[32m"
RESET="\e[39m"
BLUE="\e[34m"
RED="\e[31m"
CYAN="\e[36m"
YELLOW="\e[33m"

printf "== ${CYAN}Testing Collections${RESET} ==\n"
for test in collection_tests/*.out; do
    echo "Running $test"
    # We can't just include it like `clang libLLV.a my_file.c` like in clang
    # we have to do it this way because GCC is ancient in some regards and this
    # is one of them.
    ./$test
    if [ $? -ne 0 ]; then
        printf "${CYAN}Testing Collections${CYAN} ${RED}Failed${RESET}\n"
        printf "${RED}ERROR${RESET}: Test $test ${RED}failed${RESET}\n"
        exit 1
    fi
done

printf "== ${CYAN}Testing Collections ${GREEN}Passed${RESET} ==\n"
printf "== ${CYAN}Testing Output${RESET} ==\n"

for test in output_tests/*.out; do
    printf "\n${BLUE}Running${RESET} $test\n"
    filename=${test%.*}
    for test_case in test_matrix/*; do
        source $test_case
        ./$test > $filename.result
        test_case_name=$(basename $test_case)
        if [ ! -f $filename.expected."${test_case_name%%.*}" ]; then
            printf "${YELLOW}Missing${RESET} $filename.expected."${test_case_name%%.*}" exiting\n"
            exit 2
        fi
        diff $filename.result $filename.expected."${test_case_name%%.*}"
        if [ $? -ne 0 ]; then
            printf "${CYAN}Testing Output${CYAN} ${RED}Failed${RESET}\n"
            printf "${RED}ERROR${RESET}: Test $test:$test_case ${RED}failed${RESET} difference shown above exiting\n"
            exit 1
        fi
        printf "\n$test:$test_case ${GREEN}successful${RESET}\n"
    done
    printf "\n$test ${GREEN}successful${RESET}\n"
done

printf "\n== ${CYAN}Testing Output${CYAN} ${GREEN}Passed${RESET} ==\n"
printf "\nCleaning up tmp files\n"

rm collection_tests/*.out
rm output_tests/*.out
rm output_tests/*.result
