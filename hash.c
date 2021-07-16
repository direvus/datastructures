#include <limits.h>
#include <string.h>

/*
 * Compute an unsigned long integer hash from a byte array.
 *
 * If 'bytes' is a NULL pointer, or 'len' is less than 1, the hash is zero.
 *
 * If 'len' is 1, then the hash is just the integer value of the first byte.
 * Otherwise, we read 'len' bytes from 'bytes', and for each byte we save the
 * leftmost bit of the hash, bitshift the hash one bit left, then add the value
 * of the new byte, less the value of the bit that was shifted off, multiplied
 * by the value of the preceding byte (if not the first byte).
 */
unsigned long hash_shimmy2_len(const char *bytes, const size_t len) {
    unsigned long result = 0;
    unsigned char shift = 0;
    unsigned int ch = 0;
    const size_t bits = (sizeof result) * CHAR_BIT - 1;
    if (len == 0) {
        return 0;
    } else if (len == 1) {
        return bytes[0];
    }
    for (size_t i = 0; i < len; i++) {
        shift = result >> bits;
        result <<= 1;
        ch = bytes[i] - shift;
        if (i > 0)
            ch *= bytes[i - 1];
        result += ch;
    }
    return result;
}

/*
 * Compute an unsigned long integer hash from a NUL-terminated byte array.
 *
 * As for hash_shimmy2_len(), except we consume bytes from 'bytes' until we
 * encounter a zero byte.
 */
unsigned long hash_shimmy2(const char *bytes) {
    if (!bytes) {
        return 0;
    }
    return hash_shimmy2_len(bytes, strlen(bytes));
}
