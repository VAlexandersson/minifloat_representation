#include <stdio.h>

#define SMALLEST_VALUE  0.015625
#define LARGEST_VALUE   15.5
#define SIGN_BIT        7

#define BIAS            3
#define IS_INF(X)       (X > LARGEST_VALUE) // 0 110 1111 = 7.5
#define IS_ZERO(X)      (X < SMALLEST_VALUE)
#define GET_SIGN(X)     (X < 0 ? 0x80 : 0)


typedef struct {
    char data;
} Float8;


void    print_bits(unsigned int x);
void    float_to_float8(Float8 *out, float f);
int     outsideLimits(Float8 *out, float f);
int     get_exponent(float* f);
int     get_mantissa(float f);


int main() {
    Float8 f;
    float floats[] = {-0.015625f, -0.25f, -1.3799f, 0.140625f, 7.5f, 0.9f};
    int length = sizeof(floats)/sizeof(floats[0]);

    for(int i = 0; i < length; i++) {
        printf("%f\t", floats[i]);
        float_to_float8(&f, floats[i]);
        print_bits(f.data);
    }
    return 0;
}


void float_to_float8(Float8 *out, float f) {

    out->data = GET_SIGN(f);
    if(out->data & (1<<SIGN_BIT)) f *=-1;

    if(outsideLimits(out, f)) return;

    int exponent = get_exponent(&f);

    // Denormalized form.
    if (exponent <= -BIAS) exponent = -BIAS;

    int mantissa = get_mantissa(f);

    out->data |= (exponent+BIAS) << 4;
    out->data |= (mantissa) << 0;
}


int get_exponent(float* f) {
    int exponent = 0;
    while (*f >= 2) {
        exponent++;
        *f /= 2;
    }
    while (*f < 1 && exponent > -BIAS) {
        exponent--;
        *f *= 2;
    }
    return exponent;
}


int get_mantissa(float f) {
    int mantissa = 0;
    for(int i = 3; i >= 0; i--) {
        if(f >= 1) {
            mantissa |= 1 << i;
            f--;
        }
        f *=2 ;
    }
    return mantissa;
}


int outsideLimits(Float8 *out, float f) {
    if(IS_INF(f) || IS_ZERO(f)) {
        printf("OUTSIDE LIMITS\n");
        if(IS_INF(f)) out->data |= 0x70;
        return 1;
    }
    return 0;
}


void print_bits(unsigned int x) {
    for (int i = 7; i >= 0; i--)
        putchar(((x >> i) & 1U) ? '1' : '0');
    putchar('\n');
}

/*
 *  100.0000 = 4.0      = 110 0000
 *  10.00000 = 2.0      = 101 0000
 *  1.000000 = 1.0      = 100 0000
 *  0.100000 = 0.5      = 010 0000
 *  0.010000 = 0.25     = 001 0000
 *  0.001000 = 0.125    = 000 1000
 *  0.000100 = 0.0625   = 000 0100
 *  0.000010 = 0.03125  = 000 0010
 *  0.000001 = 0.015625 = 000 0001
 * */
