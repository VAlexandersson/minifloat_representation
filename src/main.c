#include "float8.h"
#include <stdio.h>
#include <stdlib.h>

void print_usage(const char* program_name) {
    fprintf(stderr, "Usage: %s <float_number>\n", program_name);
    fprintf(stderr, "Example: %s 1.5\n", program_name);
    fprintf(stderr, "Valid range: %f to %f\n", SMALLEST_VALUE, LARGEST_VALUE);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    char *endptr;
    float input = strtof(argv[1], &endptr);
    
    if (*endptr != '\0') {
        fprintf(stderr, "Error: Invalid float number '%s'\n", argv[1]);
        return 1;
    }

    Float8 f8;
    Float8Error error = float_to_float8(&f8, input);
    
    if (error != FLOAT8_SUCCESS && 
        error != FLOAT8_ERROR_INFINITY && 
        error != FLOAT8_ERROR_ZERO) {
        return 1;
    }

    printf("Float8 representation of %f: ", input);
    print_bits(f8.data);
    
    return 0;
}