
#ifndef TEST_FLOAT8_H
#define TEST_FLOAT8_H

#include "float8.h"

// Test result structure
typedef struct {
    int tests_run;
    int tests_failed;
} TestResult;

void run_float8_tests(TestResult* result);

#endif // TEST_FLOAT8_H