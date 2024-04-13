#include "../include/float8.h"
#include <stdio.h>

void print_bits(unsigned int x) {
  for (int i = 7; i >= 0; i--) {
    putchar(((x >> i) & 1U) ? '1' : '0');
    if (i == 7 || i == 4)
      putchar(' ');
  }
  putchar('\n');
}

Float8Error float_to_float8(Float8 *out, float f) {
  if (out == NULL) {
    printf("Error: NULL pointer passed to float_to_float8()\n");
    return FLOAT8_ERROR_NULL_PTR;
  }

  out->data = get_sign(&f);

  if (is_inf(f)) {
    out->data |= 0x70; // set inf bits
    return FLOAT8_ERROR_INFINITY;
  } else if (is_zero(f)) {
    out->data = 0;
    return FLOAT8_ERROR_ZERO;
  }

  out->data |= get_exponent(&f);
  out->data |= get_mantissa(f, fl8_exponent(out->data));
  return FLOAT8_SUCCESS;
}

Float8Error get_sign(float *f) {

  if (f == NULL) {
    printf("Error: NULL pointer passed to get_sign()\n");
    return FLOAT8_ERROR_NULL_PTR;
  }
  if (*f > 0)
    return 0;
  *f *= -1;
  return 0x80;
}

int get_exponent(float *f) {
  int exponent = 0;
  while (*f >= 2) {
    exponent++;
    *f /= 2;
  }
  while (*f < 1 && exponent > -BIAS) {
    exponent--;
    *f *= 2;
  }
  return (exponent + BIAS) << MANTISSA_BITS;
}

int get_mantissa(float f, int exp) {
  int mantissa = 0;
  if (exp != 0)
    f = (f - 1) * 2; // removes implicit 1 from norm.form.
  for (int i = MANTISSA_BITS - 1; i >= 0; i--) {
    if (f >= 1) {
      mantissa |= 1 << i;
      f--;
    }
    f *= 2;
  }
  return (mantissa) << 0;
}

int is_inf(float f) { return (f > LARGEST_VALUE || f < -LARGEST_VALUE); }

int is_zero(float f) { return (f < SMALLEST_VALUE && f > -SMALLEST_VALUE); }

int fl8_exponent(int f) { return (int)((f & 0x70) >> 4) << 0; }

int outside_limits(Float8 *out, float f) {
  if (is_inf(f) || is_zero(f)) {
    if (is_inf(f))
      out->data |= 0x70; // set inf bits
    else
      out->data = 0;
    return 1;
  } else
    return 0;
}

void process_floats(const float *floats, int length) {
  Float8 f;
  for (int i = 0; i < length; i++) {
    printf("%f\t", floats[i]);
    float_to_float8(&f, floats[i]);
    print_bits(f.data);
    if ((i + 1) % 3 == 0)
      printf("\n");
  }
}