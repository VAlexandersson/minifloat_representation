#include <stdio.h>

#define SMALLEST_VALUE  0.015625
#define LARGEST_VALUE   15.5
#define BIAS            3


typedef struct {
    char data;
} Float8;


void    print_bits(unsigned int);
void    float_to_float8(Float8*, float);
int     outsideLimits(Float8*, float*);
int     get_exponent(float*);
int     get_mantissa(float);
int     get_sign(float*);

int     is_inf(float* f)    { return (*f > LARGEST_VALUE ); }
int     is_zero(float* f)   { return (*f < SMALLEST_VALUE); }

int main() {
    Float8 f;
    float floats[] = {-0.015625f, -0.25f, -1.3799f, 0.140625f, 7.5f, 0.9f, -45.77};
    int length = sizeof(floats)/sizeof(floats[0]);

    for(int i = 0; i < length; i++) {
        printf("%f\t", floats[i]);
        float_to_float8(&f, floats[i]);
        print_bits(f.data);
    }
    return 0;
}



void float_to_float8(Float8 *out, float f) {

    out->data = get_sign(&f);

    if(outsideLimits(out, &f)) return;

    int exponent = get_exponent(&f);

    // Denormalized form.
    if (exponent <= -BIAS) exponent = -BIAS;

    int mantissa = get_mantissa(f);

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


int outsideLimits(Float8 *out, float *f) {
    if(is_inf(f) || is_zero(f)) {
        if(is_inf(f)) out->data |= 0x70;
        return 1;
    }
    return 0;
}


void print_bits(unsigned int x) {
    for (int i = 7; i >= 0; i--)
        putchar(((x >> i) & 1U) ? '1' : '0');
    putchar('\n');
}
