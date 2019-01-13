#!/usr/bin/env bash

# PLEASE only use this on a test case once you know
# that the test case is producing correct output
# else you will just cause false positives to pop up

# For a single .out and multiple .in generates expected

GREEN="\e[32m"
RESET="\e[39m"
BLUE="\e[34m"

for test in test_matrix/*; do
    source $test
    file=$(basename $test)
    printf "== ${GREEN}Generating${RESET} $test ==\n"
    for f in ${@:2}; do
        printf "    ${BLUE}For${RESET} $f\n"
        num_and_in="${f#*.}"
        num="${num_and_in%%.*}"
        basename="$(basename $f)"
        $1 $num < $f > $(dirname $f)/expected/${basename%%.*}.$num.expected."${file%%.*}"
    done
done
