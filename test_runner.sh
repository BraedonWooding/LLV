#!/usr/bin/env bash

# Our Testing Script
# Runs the collection tests as well as the output tests
# Collection tests are orientated around just checking that the collections work as expected
# Output tests just check the program hasn't regressed and test all the examples and more!

if [ -z $CC ]; then CC=clang; fi

shopt -s nullglob

GREEN="\e[32m"
RESET="\e[39m"
BLUE="\e[34m"
RED="\e[31m"
CYAN="\e[36m"
YELLOW="\e[33m"

printf "\n== ${CYAN}Testing Collections${RESET} ==\n\n"
total_tests=0
total_asserts=0

for test in $1/collection_tests/*.out; do
    # We can't just include it like `clang libLLV.a my_file.c` like in clang
    # we have to do it this way because GCC is ancient in some regards and this
    # is one of them.
    test_basename="$(basename $test).log"
    log_filename="${test_basename%%.*}.log"
    exec 5>&1
    log_out=$($test $log_filename|tee >(cat - >&5))
    array=(`echo "$log_out" | awk -F '[()]' '{print $2}' | awk 'BEGIN {sum_tests=0;sum_asserts=0}; {sum_tests+=$1;sum_asserts+=$5}; END {printf "%d %d\n", sum_tests, sum_asserts}'`)
    num_tests=${array[0]}
    let total_tests+=num_tests
    let total_asserts+=${array[1]}

    if [ $? -ne 0 ]; then
        printf "\n== ${CYAN}Testing Collections${CYAN} ${RED}Failed${RESET} ==\n"
        printf "${RED}ERROR${RESET}: Test $test ${RED}failed${RESET}\n"
        printf "Log File $(pwd)/$log_filename:\n\n"
        cat $log_filename
        exit 1
    fi
done

printf "\nA total of ${CYAN}$total_tests tests${RESET} were run for a grand total of ${CYAN}$total_asserts asserts${RESET} being done\n"

printf "\n== ${CYAN}Testing Collections ${GREEN}Passed${RESET} ==\n"
printf "== ${CYAN}Testing Output${RESET} ==\n\n"

for test in $1/output_tests/*.out; do
    base=$(basename $test)
    filename=${base%.*}
    printf "${BLUE}Running${RESET} $filename "
    for test_case in test_matrix/*; do
        source $test_case
        $test > $filename.result
        test_case_name=$(basename $test_case)
        if [ ! -f output_tests/expected/$filename.expected."${test_case_name%%.*}" ]; then
            printf "\n${YELLOW}Missing${RESET} output_tests/expected/$filename.expected."${test_case_name%%.*}" exiting\n"
            exit 2
        fi
        diff $filename.result output_tests/expected/$filename.expected."${test_case_name%%.*}"
        if [ $? -ne 0 ]; then
            printf "\n== ${CYAN}Testing Output${CYAN} ${RED}Failed${RESET} ==\n"
            printf "${RED}ERROR${RESET}: Test $test:$test_case ${RED}failed${RESET} difference shown above exiting\n"
            exit 1
        fi
        printf "${GREEN}.${RESET}"
    done
    printf " ${GREEN}successful${RESET}\n"
done

printf "\n== ${CYAN}Testing Output${CYAN} ${GREEN}Passed${RESET} ==\n"
printf "== ${CYAN}Testing Examples${RESET} ==\n\n"

for test in example/example_tests/*.in; do
    base=$(basename $test)
    filename=${base%.*}
    printf "${BLUE}Running${RESET} ${base%%.*} "
    for test_case in test_matrix/*; do
        source $test_case
        num_and_in="${test#*.}"
        num="${num_and_in%%.*}"
        $1/example_tests $num < $test > $filename.result
        test_case_name=$(basename $test_case)
        if [ ! -f example/example_tests/expected/$filename.expected."${test_case_name%%.*}" ]; then
            printf "\n${YELLOW}Missing${RESET} example/example_tests/expected/$filename.expected."${test_case_name%%.*}" exiting\n"
            exit 2
        fi
        diff $filename.result example/example_tests/expected/$filename.expected."${test_case_name%%.*}"
        if [ $? -ne 0 ]; then
            printf "\n== ${CYAN}Testing Example${CYAN} ${RED}Failed${RESET} ==\n"
            printf "${RED}ERROR${RESET}: Test $test:$test_case ${RED}failed${RESET} difference shown above exiting\n"
            exit 1
        fi
        printf "${GREEN}.${RESET}"
    done
    printf " ${GREEN}successful${RESET}\n"
done

source default.sh

printf "\n== ${CYAN}Testing Examples${CYAN} ${GREEN}Passed${RESET} ==\n"
printf "\nCleaning up tmp files\n"

rm *.result

shopt -u nullglob
