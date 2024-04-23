# Floating-point representation - Minifloat

Emulating an 8-bit floating-point representation 'Float8' that has a 4-bit mantissa, 3-bit exponent, 1-bit sign and a -3 exponent bias.

## Valid Range
The Float8 format supports numbers in the range of 0.015625 to 15.5. Values outside this range will be clamped to the nearest representable number.

## Building and Running

### Prerequisites
- CMake (version 3.10 or higher)
- C compiler (gcc or clang)

### Build Instructions
1. Create a build directory and navigate to it:
```bash
mkdir build && cd build # create build dir. and change to it
cmake .. # generate build files using cmake
make # build the project with make
```

### Running Float8

The program takes a single floating-point number as an argument:
```bash
./float8 <float_number>
```

Example usage:

```bash
./float8 1.5
```

This will output the binary representation of the float number in Float8 format:

```
Float8 representation of 1.500000: 0 100 1000
```

- First bit: Sign (0 = positive, 1 = negative)
- Next 3 bits: Exponent
- Last 4 bits: Mantissa

Valid input range: 0.015625 to 15.5 

#### Running the tests

To run the test just run: 
```bash
./build/float8_test
```

## Float8 properties and "rules"

### The bits

The float's 8-bits are distributed as follows:
  - 1-bit sign
  - 3-bit exponent
  - 4-bit mantissa

#### 1.  Biased Exponent
  - The exponent is biased is there to make it possible to represent numbers that goes towards 0. With a -3 biased we will get a exponent range of $(-3) - 4$(, instead of $0 - 7$). 

#### 2. Implicit binary dot, $0 0 0 0 \implies 0 . 0 0 0$ 
  - Implicit binary dot after leading digit in the mantissa. 
  
#### 3. Implicit leading '1'in mantissa, $0 . 0 0 0 \implies 1 . 0 0 0 0$. 
  - The mantissa will have an implicit 1 in front of itself resulting in an extra bit of precision.
  
### Special cases, if exp. = 0 0 0 & 1 1 1.

#### If exponent = 0 0 0 (-3) $\implies$ Denormalized form
  - Implicit '0' instead of implicit '1'.
  - Exponent bias = '-2' instead of '-3'

- Resulting in that '0' and values closer to '0' (0.015625) can now be represented in this denormalized form.
  - $[0\ 000\ 0000] = 2^{-2} \cdot 0.0000_2 = 0$
  - $[0\ 000\ 0001] = 2^{-2} \cdot 0.0001_2 = 0.000001_2\ (= 0.015625)$

#### If exponent = 1 1 1 (4)
3 different cases here:

1. \[0 111 0000\] = +Infinity
2. \[1 111 0000\] = -Infinity
3. Mantissa != 0000 $\implies$ NaN, not a number. *\[0 111 0001\] ... \[1 111 1111\] = NaN.*

- Resulting in largest unsigned value is 15.5
  - $[0\ 011\ 1111] = \pm 2^3 \cdot 1.1111_2 = \pm 1111.1_2 = \pm 15.5$

