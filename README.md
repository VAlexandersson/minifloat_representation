# Floating-point representation - Minifloat

Emulating an 8-bit floating-point representation that has a 1-bit mantissa, 4-bit exponent and 3-bit exponent. The exponent has also a -3 bias, meaning that the exponent's value will range from $0 - 7$ to $(-3) - 4$.

The 8-bit float will be stored in the C struct 'Float8' that holds a char that will act as the data buffer for the floating-point number.  

```C
  typedef struct {
    char data;
  } Float8;
```

2. The representation's 8-bits will be distributed as follows:  
  - 1-bit for sign
  - 4-bit mantissa
  - 3-bit exponent

The representation will also have a -3 bias for the exponent meaning the exponent's range will go from $0 - 7$ to $(-3) - 4$.

What are the smallest/largest numbers you can represent? What is the smallest increment?
