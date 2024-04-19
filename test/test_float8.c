
#include "test_float8.h"
#include <stdio.h>

#define TEST(name, test) \
    do { \
        printf("Test %s: ", name); \
        result->tests_run++; \
        if (test) { \
            printf("PASSED\n"); \
        } else { \
            printf("FAILED\n"); \
            result->tests_failed++; \
        } \
    } while (0)

typedef struct {
    float input;
    unsigned char expected;
} TestCase;

static const TestCase test_cases[] = {
    {0.001f,     0x00}, // 0 000 0000
    {-20.0f,     0xF0}, // 1 111 0000
    {55.8f,      0x70}, // 0 111 0000 (infinity)
    {15.5f,      0x6F}, // 0 110 1111
    {15.49f,     0x6E}, // 0 110 1110
    {14.9f,      0x6D}, // 0 110 1101
    {0.25f,      0x10}, // 0 001 0000
    {-0.296875f, 0x93}, // 1 001 0011
    {0.304688f,  0x13}, // 0 001 0011
    // Denormalized numbers
    {0.234375f,  0x0F}, // 0 000 1111
    {0.015625f,  0x01}, // 0 000 0001
    {0.187500f,  0x0C}, // 0 000 1100
};

static int test_conversion(const TestCase* tc) {
    Float8 result;
    float_to_float8(&result, tc->input);
    if (result.data != tc->expected) {
        printf("\nExpected: "); print_bits(tc->expected);
        printf("Got:      "); print_bits(result.data);
        return 0;
    }
    return 1;
}

void run_float8_tests(TestResult* result) {
    result->tests_run = 0;
    result->tests_failed = 0;

    for (size_t i = 0; i < sizeof(test_cases)/sizeof(test_cases[0]); i++) {
        char test_name[64];
        snprintf(test_name, sizeof(test_name), "Converting %f", test_cases[i].input);
        TEST(test_name, test_conversion(&test_cases[i]));
    }

    printf("\nTest Summary:\n");
    printf("Tests run: %d\n", result->tests_run);
    printf("Tests failed: %d\n", result->tests_failed);
}