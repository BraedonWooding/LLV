#!/usr/bin/env bash

# PLEASE only use this on a test case once you know
# that the test case is producing correct output
# else you will just cause false positives to pop up

GREEN="\e[32m"
RESET="\e[39m"
BLUE="\e[34m"

for test in test_matrix/*; do
    source $test
    file=$(basename $test)
    printf "== ${GREEN}Generating${RESET} $test ==\n"
    for f in $@; do
        printf "    ${BLUE}For${RESET} $f\n"
        b=$(basename $f)
        $f > output_tests/expected/"${b%%.*}".expected."${file%%.*}"
    done
done
