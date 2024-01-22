#ifndef FLOAT8_H
#define FLOAT8_H

// Custom 8-bit floating point representation
typedef struct {
    unsigned char data;
} Float8;

// Functions
void process_floats(const float *floats, int length); 
void print_bits(unsigned int x);
void float_to_float8(Float8 *out, float f);
int get_sign(float *f);
int get_exponent(float *f);
int get_mantissa(float f, int exp);
int is_inf(float f);
int is_zero(float f);
int fl8_exponent(int f);
int outside_limits(Float8 *out, float f);

#endif // FLOAT8_H