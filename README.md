# Floating-point representation - Minifloat

Emulating an 8-bit floating-point representation 'Float8' that has a 1-bit mantissa, 4-bit exponent, 3-bit exponent and a -3 exponent bias.

- Largest unsigned value: 
  - $\[0\ 011\ 1111\] = \pm 2^3 \cdot 1.1111_2 = \pm 1111.1_2 = \pm 15.5$
- Smallest unsigned value:
  - $\[0\ 000\ 0001\] = \pm 2^{-2} \cdot 0.0001_2 = \pm 0.000001_2 = \pm 0.015625$

## Float8 properties and "rules"

### The bits

The float's 8-bits are distributed as follows:
  - 1-bit sign
  - 3-bit exponent
  - 4-bit mantissa

#### 1.  Biased Exponent
  - The exponent is biased with -3 so numbers that are closer to 0 can be represented. This changes the exponent range from $0 - 7$ to $(-3) - 4$. 

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
  - $\[0\ 011\ 1111\] = \pm 2^3 \cdot 1.1111_2 = \pm 1111.1_2 = \pm 15.5$
