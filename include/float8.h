#ifndef FLOAT8_H
#define FLOAT8_H

// Constants
#define SMALLEST_VALUE  0.015625
#define LARGEST_VALUE   15.5
#define BIAS            3
#define MANTISSA_BITS   4

// Custom 8-bit floating point representation
typedef struct {
    unsigned char data;
} Float8;

// Error codes
typedef enum {
    FLOAT8_SUCCESS = 0,       // No error
    FLOAT8_ERROR_NULL_PTR,    // Null pointer error
    FLOAT8_ERROR_OVERFLOW,    // Overflow error
    FLOAT8_ERROR_UNDERFLOW,   // Underflow error
    FLOAT8_ERROR_INFINITY,    // Infinity encountered
    FLOAT8_ERROR_ZERO,        // Zero value error
} Float8Error;

/**
 * @brief Prints binary representation of a Float8 number
 * @param x The Float8 value to print
 * Format: SEEE MMMM (S=Sign, E=Exponent, M=Mantissa)
 */
void print_bits(unsigned int x);

/**
 * @brief Converts IEEE 754 float to Float8 format
 * @param out Pointer to store the resulting Float8
 * @param f Input float value
 * @return Error code indicating success or specific failure
 */
Float8Error float_to_float8(Float8 *out, float f);

/**
 * @brief Extracts the sign bit from a float value
 * @param f Pointer to float value (will be modified)
 * @return Float8Error 0x80 if negative, 0 if positive, or error code
 * 
 * Takes a float pointer, determines its sign, and modifies the float
 * to be positive for further processing. Returns the appropriate 
 * sign bit for Float8 format.
 * 
 * Example:
 *   float f = -3.14;
 *   uint8_t sign = get_sign(&f); // Returns 0x80, f becomes 3.14
 */
Float8Error get_sign(float *f);

/**
 * @brief Calculates the unbiased exponent for Float8 format
 * @param f Pointer to float value (will be modified)
 * @return int Unbiased exponent value
 * 
 * Normalizes the float value between [1,2] and tracks required
 * exponent shifts. Handles the Float8 format's limited range
 * with respect to the bias.
 * 
 * Example:
 *   float f = 8.0;
 *   int exp = get_exponent(&f); // Returns 3, f becomes 1.0
 */
int get_exponent(float *f);

/**
 * @brief Extracts mantissa bits from normalized float
 * @param f Normalized float value (1.0 <= f < 2.0)
 * @param exp_bits Exponent bits for alignment
 * @return uint8_t 4-bit mantissa value
 * 
 * Extracts 4 mantissa bits from normalized float,
 * considering position after the binary point.
 * 
 * Example:
 *   float f = 1.5;  // Binary: 1.1000...
 *   uint8_t m = get_mantissa(f, 0x60); // Returns 0x8
 */
int get_mantissa(float f, int exp);

/**
 * @brief Checks if the float value is infinity
 * @param f Input float value
 * @return int 1 if the value is infinity, 0 otherwise
 */
int is_inf(float f);

/**
 * @brief Checks if the float value is zero
 * @param f Input float value
 * @return int 1 if the value is zero, 0 otherwise
 */
int is_zero(float f);

/**
 * @brief Converts unbiased exponent to Float8 biased format
 * @param exp Raw exponent value
 * @return uint8_t Biased exponent in Float8 format (3 bits)
 * 
 * Applies bias and formats exponent to fit in 3 bits.
 * Float8 uses bias of -3 to maximize range.
 * 
 * Example:
 *   uint8_t biased = fl8_exponent(3); // Returns 0x60
 */
int fl8_exponent(int f);

/**
 * @brief Checks if the float value is outside the representable range of Float8
 * @param out Pointer to store the resulting Float8
 * @param f Input float value
 * @return int 1 if the value is outside limits, 0 otherwise
 * 
 * This function checks if the float value is either infinity or zero,
 * and sets the appropriate bits in the Float8 representation.
 */
int outside_limits(Float8 *out, float f);

/**
 * @brief Processes an array of floats and prints their Float8 representations
 * @param floats Array of float values
 * @param length Number of elements in the array
 * 
 * This function converts each float in the array to Float8 format,
 * prints the original float value and its binary representation.
 * It prints three values per line.
 */
void process_floats(const float *floats, int length);

#endif // FLOAT8_H