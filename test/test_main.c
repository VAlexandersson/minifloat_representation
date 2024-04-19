
#include "test_float8.h"
#include <stdlib.h>

int main() {
    TestResult result = {0, 0};
    run_float8_tests(&result);
    return result.tests_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}