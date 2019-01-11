#!/usr/bin/env bash

# PLEASE only use this on a test case once you know
# that the test case is producing correct output
# else you will just cause false positives to pop up

cmake .
make

GREEN="\e[32m"
RESET="\e[39m"
BLUE="\e[34m"

for test in test_matrix/*; do
    source $test
    file=$(basename $test)
    printf "== ${GREEN}Generating${RESET} $test ==\n"
    for f in $@; do
        printf "    ${BLUE}For${RESET} $f\n"
        $f > "${f%%.*}".expected."${file%%.*}"
    done
done
