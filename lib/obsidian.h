/* 
    MIT License

    Copyright (c) 2018 Braedon Wooding

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef BWW_OBSIDIAN_H
#define BWW_OBSIDIAN_H

#ifndef OBSIDIAN_LOG_SIZE
#define OBSIDIAN_LOG_SIZE 1024
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#ifndef STRINGIFY
#define STRINGIFY(x) #x
#endif

#define OBS_SETUP(name) {\
    int successes = 0; \
    int failures = 0; \
    int num_groups = 0; \
    int num_tests = 0; \
    int groups_passed = 0; \
    bool success = false; \
    int old_successes = 0; \
    int old_failures = 0; \
    int old_tests = 0; \
    int tests_in_group = 0; \
    int successes_in_group = 0; \
    int failures_in_group = 0; \
    int num_of_asserts = 0; \
    int old_num_of_asserts = 0; \
    char log[OBSIDIAN_LOG_SIZE] = {0}; \
    printf("Beginning Tests for " #name "\n\n");

#define OBS_TEST_GROUP(group_name, group) \
    num_groups++; \
    old_successes = successes; \
    old_failures = failures; \
    old_tests = num_tests; \
    old_num_of_asserts = num_of_asserts; \
    printf("== " #group_name " ==\n\n"); \
    group \
    tests_in_group = num_tests - old_tests; \
    successes_in_group = successes - old_successes; \
    failures_in_group = failures - old_failures; \
    if (failures_in_group == 0) { \
        groups_passed++; \
        printf(GRN "Passed" RESET " %d/%d tests\n\n", successes_in_group, tests_in_group); \
    } else { \
        printf(RED "Failed" RESET " %d/%d tests\n\n", failures_in_group, tests_in_group); \
    } \
    printf(BLU "Ran" RESET " a total of %d checks in this group\n", num_of_asserts - old_num_of_asserts);

#define OBS_TEST(name, group) \
    num_tests++; \
    success = true; \
    memset(log, 0, sizeof(log)); \
    group \
    if (success) { \
        successes++; \
        printf(#name GRN " succeeded\n" RESET); \
    } else { \
        failures++; \
        printf(#name RED " failed" RESET "...\n"); \
        printf("\n== Log ==\n\n%s\n", log); \
        memset(log, 0, sizeof(log)); \
    } \

#define GET_FORMAT(x, op, val, y) \
    _Generic((val), \
            long: x "%ld " #op " %ld" y, \
            long long: x "%lld " #op " %lld" y, \
            unsigned int: x "%u " #op " %u" y, \
            unsigned long: x "%lu " #op " %lu" y, \
            unsigned long long: x "%llu " #op " %llu" y, \
            int: x "%d " #op " %d" y, \
            float: x "%f " #op " %f" y, \
            double: x "%lf " #op " %lf" y, \
            long double: x "%llf " #op " %llf" y, \
            char: x "%c " #op " %c" y, \
            short: x "%hd " #op " %hd" y, \
            unsigned short: x "%hu" #op "%hu" y, \
            char *: x "%s " #op " %s" y, \
            bool: x "%d " #op " %d" y, \
            default: x "%p " #op " %p" y \
    )

#define obs_test(val, op, cond) \
    { \
    num_of_asserts++; \
    if (success && !(((val) op (cond)))) { \
        success = false; \
        sprintf(log,  GET_FORMAT(__FILE__ ":%d failed because " #val " => ", op, val, " is false"), __LINE__, val, cond); \
    } \
    }

#define obs_test_true(val) \
    { \
    num_of_asserts++; \
    if (success && !(val)) { \
        success = false; \
        sprintf(log,  __FILE__ ":%d failed because " #val " is false", __LINE__); \
    } \
    }

#define obs_test_false(val) \
    { \
    num_of_asserts++; \
    if (success && (val)) { \
        success = false; \
        sprintf(log,  __FILE__ ":%d failed because " #val " is true", __LINE__); \
    } \
    }

#define obs_test_eql(val, cond) obs_test(val, ==, cond)
#define obs_test_neql(val, cond) obs_test(val, !=, cond)
#define obs_test_lt(val, cond) obs_test(val, <, cond)
#define obs_test_gt(val, cond) obs_test(val, >, cond)
#define obs_test_lte(val, cond) obs_test(val, <=, cond)
#define obs_test_gte(val, cond) obs_test(val, >=, cond)

#define obs_test_strcmp(a, b) \
    { \
    if (success && strcmp(a, b)) { \
        success = false; \
        sprintf(log,  __FILE__ ":%d failed because %s != %s", __LINE__, a, b); \
    } \
    }

#ifndef NDEBUG
#define obs_assert(val, op, cond) \
    { \
        if (!((val) op (cond))) { \
            fprintf(stderr, GET_FORMAT(__FILE__ ":%d failed because " #val " => ", op, val, " <= " #cond " is false"), __LINE__, val, cond); \
            abort(); \
        } \
    }
#define obs_assert_strcmp(a, b) \
    { \
        if (strcmp(a, b)) { \
            fprintf(stderr, __FILE__ ":%d failed because %s != %s", __LINE__, a, b); \
            abort(); \
        } \
    }
#else
#define obs_assert(val, op, cond) 
#define obs_assert_strcmp(a, b) 
#endif

#define obs_assert_eql(val, cond) obs_assert(val, ==, cond)
#define obs_assert_neql(val, cond) obs_assert(val, !=, cond)
#define obs_assert_lt(val, cond) obs_assert(val, <, cond)
#define obs_assert_gt(val, cond) obs_assert(val, >, cond)
#define obs_assert_lte(val, cond) obs_assert(val, <=, cond)
#define obs_assert_gte(val, cond) obs_assert(val, >=, cond)

// Present results and exits
#define OBS_REPORT \
    printf("--------\n"); \
    printf("\nTests have finished\n"); \
    printf(BLU "Ran" RESET " a total of %d checks\n", num_of_asserts); \
    if (groups_passed == num_groups) { \
        printf(GRN "Passed" RESET " %d/%d groups\n", groups_passed, num_groups); \
    } else { \
        printf(RED "Failed" RESET " %d/%d groups\n", num_groups - groups_passed, num_groups); \
    } \
    if (successes == num_tests) { \
        printf(GRN "Passed" RESET " %d/%d tests\n", successes, num_tests); \
        printf("OK\n"); \
    } else { \
        printf(RED "Failed" RESET " %d/%d tests\n", failures, num_tests); \
    } \
    return successes != num_tests; \
}

#endif