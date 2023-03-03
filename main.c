#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>

#define SMALLEST_VALUE 0.015625
#define LARGEST_VALUE 7.5
#define SMALLEST_NORM 0.25


#define SIGN_BIT        7
#define EXPONENT_BITS   3
#define MANTISSA_BITS   4

#define BIAS            (-3)
#define IS_INF(X) (X > LARGEST_VALUE) // 0 110 1111 = 7.5

#define IS_ZERO(X) (X < SMALLEST_VALUE)
#define GET_SIGN(X) (X < 0 ? 0x80 : 0)



typedef struct {
    char data;
} Float8;


void print_bits(unsigned int x);
void convert_float(Float8 *out, float f);
int fractionToBinary(float fraction, char* binary_fraction);
int outsideLimits(Float8 *out, float f);
void decimal_to_binary(int n, char *binary_str);




int fractionToBinary(float fraction, char* binary_fraction){

    int i = 0; // dot position

    while (fraction > 0 && i < MANTISSA_BITS+2) {
        fraction *= 2;
        if (fraction >= 1) {
            binary_fraction[i] = '1';
            fraction -= 1;
        }
        else binary_fraction[i] = '0';
        i++;
    }
    return i+1;
}


void convert_float(Float8 *out, float f) {
/*
    printf("%f\n", f);
    f = fabsf(f);
    printf("%f\n", f);

    if(IS_INF(f) || IS_ZERO(f)) {
        if(IS_INF(f)) out->data |= 0x70;
        return;
    }*/
    printf("---\n\n---\n%f\n", f);
    out->data = GET_SIGN(f);
    if(out->data & (1<<7)) f *=-1;

    if(outsideLimits(out, f)) return;

    int whole_part = (int)floorf(f);
    float fraction = f - whole_part;



    /*          CAN BE REMOVED  */
    /* >>>>>>>>>>>>FROM HERE<<<<<<<<<< */
    char whole_part_binary[4] = "";
    decimal_to_binary(whole_part, whole_part_binary);

    char binary_fraction[7] = "";
    fractionToBinary(fraction, binary_fraction);

    char binary_numb[8] = "";
    strcat(binary_numb, strlen(whole_part_binary) > 0 ? whole_part_binary : "0");
    strcat(binary_numb, ".");
    strcat(binary_numb, binary_fraction);
    printf("%s\n\n", binary_numb);
    /* >>>>>>>TO HERE<<<<<< */



    int exponent = 0; // 011 1111 = 1.9375
    while (f >= 2) {
        exponent++;
        f /= 2;
    }
    while (f < 1 && exponent > BIAS) {
        exponent--;
        f *= 2;
    }
    int mantissa = roundf((f - 1) * (1 << MANTISSA_BITS));

    /*DENORMALIZED FORM*/
    if (exponent <= BIAS) {
        mantissa = 0;
        exponent = BIAS;
        for(int i = 3; i >= 0; i--) {
            if(f >= 1) {
                mantissa |= 1 << i;
                f--;
            }
            f *=2 ;
        }
    }
    out->data |= (exponent+3) << 4;
    out->data |= (mantissa) << 0;

    print_bits(out->data);
    printf("EXPONENT\t%d\n", exponent);
    printf("MANTISSA\t%d\n", mantissa);

    //print_bits(mantissa);

}



int main() {
    Float8 f;

    convert_float(&f, -0.015625f);
    convert_float(&f, -0.25f);
    convert_float(&f, (-1.125f)); // FAULTY??
    convert_float(&f, 0.140625f);


    return 0;
}

int outsideLimits(Float8 *out, float f) {
    if(IS_INF(f) || IS_ZERO(f)) {
        printf("OUTSIDE LIMITS\n");
        if(IS_INF(f)) out->data |= 0x70;
        return 1;
    }
    return 0;
}

void decimal_to_binary(int n, char *binary_str) {
    int binary[32];
    int i = 0;
   // printf("%d\n%s\n", n, binary_str);
    while (n > 0) {
        binary[i] = n % 2;
        n = n / 2;
        i++;
    }

    for (int j = i - 1; j >= 0; j--) {
        binary_str[i - 1 - j] = binary[j] + '0';
    }
   // printf("%d\n%s\n", n, binary_str);
    binary_str[i] = '\0';
}





void print_bits(unsigned int x) {
    int i;
    for (i = CHAR_BIT * sizeof(char) - 1; i >= 0; i--)
    {
        putchar(((x >> i) & 1U) ? '1' : '0');
        if (i % CHAR_BIT == 0)
        {
            putchar(' ');
        }
    }
    putchar('\n');
}

/*
 *
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
