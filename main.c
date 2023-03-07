#include <stdio.h>

#define SMALLEST_VALUE  0.015625
#define LARGEST_VALUE   15.5
#define BIAS            3
#define MANTISSA_BITS   4

const float floats[] = {
        0.001f, -20, 55.8f,             // 0 000 0000, 1 111 0000, 0 111 0000
        15.5f, 15.49f, 14.9f,           // 0 110 1111, 0 110 1110, 0 110 1101
        0.25f, -0.296875, 0.304688,     // 0 001 0000, 1 001 0011, 0 001 0011
        // Denormalized
        0.234375f, 0.015625f, 0.187500f // 0 000 1111, 0 000 0001, 0 000 1100
};

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

int     is_inf(float f)     { return (f > LARGEST_VALUE || f < -LARGEST_VALUE); }
int     is_zero(float f)    { return (f < SMALLEST_VALUE && f > -SMALLEST_VALUE); }
int     fl8_exponent(int f) { return (int)((f & 0x70) >> 4) << 0;}

int main() {
    Float8 f = Float8_default;

    int length = sizeof(floats)/sizeof(floats[0]);

    for(int i = 0; i < length; i++) {
        printf("%f\t", floats[i]);
        float_to_float8(&f, floats[i]);
        print_bits(f.data);

        if((i+1) % 3 == 0) printf("\n");
    }
    return 0;
}

void float_to_float8(Float8 *out, float f) {
    out->data = get_sign(&f);

    if(outsideLimits(out, f)) return;

    out->data |= get_exponent(&f);
    out->data |= get_mantissa(f, fl8_exponent(out->data));
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
    return (exponent+BIAS) << MANTISSA_BITS;
}

int get_mantissa(float f, int exp) {
    int mantissa = 0;
    if(exp != 0) f = (f - 1) * 2; // removes implicit 1 from norm.form.
    for(int i = MANTISSA_BITS-1; i >= 0; i--) {
        if(f >= 1) {
            mantissa |= 1 << i;
            f--;
        }
        f *=2 ;
    }
    return (mantissa) << 0;
}

int outsideLimits(Float8 *out, float f) {
    if(is_inf(f) || is_zero(f)) {
        if(is_inf(f)) out->data |= 0x70; // set inf bits
        else out->data = 0;
        return 1;
    }
    else return 0;
}

void print_bits(unsigned int x) {
    for (int i = 7; i >= 0; i--){
        putchar(((x >> i) & 1U) ? '1' : '0');
        if(i == 7 || i == 4) putchar(' ');
    }
    putchar('\n');
}
// 110 = 15.5       < x < 8         (0.5)
// 101 = 7.75       < x < 4         (0.25)
// 100 = 3.875      < x < 2         (0.125)
// 011 = 1.9375     < x < 1         (0.0625)
// 010 = 0.96875    < x < 0.5       (0.03125)
// 001 = 0.484375   < x < 0.25      (0.015625)
// 000 = 0.234375   < x < 0.015625  (0.015625)
