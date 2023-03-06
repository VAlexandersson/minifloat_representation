#include <stdio.h>

#define SMALLEST_VALUE  0.015625
#define LARGEST_VALUE   15.5
#define BIAS            3


struct Float8_s{
    char data;
} Float8_default = {0};

typedef struct Float8_s Float8;

void    print_bits(unsigned int);
void    float_to_float8(Float8*, float);
int     outsideLimits(Float8*, float);
int     get_sign(float*);
int     get_exponent(float*);
int     get_mantissa(float, int);

int     is_inf(float f)    { return (f > LARGEST_VALUE || f < -LARGEST_VALUE); }
int     is_zero(float f)   { return (f < SMALLEST_VALUE && f > -SMALLEST_VALUE); }

int main() {
    Float8 f = Float8_default;

    float floats[] = {
            0.001f, 0, -20, 55.8, // infs and 0s
            15.5f, 14.9f, // 0 110 1111, 0 110 1101
            0.25f, -0.25f - 0.015625f * 3, // 0 001 0000, 1 001 0011
            // Denormalized
            0.234375f, 0.015625f, 0.015625f*12 // 0 000 1111, 0 000 0001, 0 000 1100
    };
    int length = sizeof(floats)/sizeof(floats[0]);

    for(int i = 0; i < length; i++) {
        printf("%f\t", floats[i]);
        float_to_float8(&f, floats[i]);
        print_bits(f.data);
    }
    return 0;
}



void float_to_float8(Float8 *out, float f) {

    int exponent, mantissa;

    out->data = get_sign(&f);

    if(outsideLimits(out, f)) return;

    //out->data |= ((get_exponent(&f)+BIAS) << 4);
    exponent = get_exponent(&f);

    //exponent = get_exponent(&f);
    //if (exponent <= -BIAS) exponent = -BIAS;

    mantissa = get_mantissa(f, exponent);

    out->data |= (exponent+BIAS) << 4;
    out->data |= (mantissa) << 0;
}

int get_sign(float* f) {
    if(*f > 0) return 0;
    *f *= -1;
    return 0x80;
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
    // Check if normalized or denormalized .
    return (exponent <= -BIAS) ? -BIAS : exponent;
}

int get_mantissa(float f, int exp) {
    int mantissa = 0;

    for(int i = (exp == -BIAS) ? 3 : 4; i >= 0; i--) { // if denorm i = 3 else i = 4
        if(f >= 1) {
            mantissa |= 1 << i;
            f--;
        }
        f *=2 ;
    }
    return (mantissa & 15);
}

int outsideLimits(Float8 *out, float f) {
    if(is_inf(f) || is_zero(f)) {
        if(is_inf(f)) out->data |= 0x70; // set inf bits
        return 1;
    }
    else return 0;
}

void print_bits(unsigned int x) {
    for (int i = 7; i >= 0; i--)
        putchar(((x >> i) & 1U) ? '1' : '0');
    putchar('\n');
}
// 110 = 15.5    < x < 8 (0.5)
// 101 = 7.75   < x < 4 (0.25)
// 100 = 3.875  < x < 2 (0.125)
// 011 = 1.9375 < x < 1 (0.0625)
// 010 = 0.96875 < x < 0.5 (0.03125)
// 001 = 0.484375 < x < 0.25 (0.015625)
// 000 = 0.234375 < x < 0.015625 (0.015625)