#include <string.h>

/*
 * Compute an unsigned integer hash from a NUL-terminated byte array.
 *
 * If the argument is a NULL pointer, or an empty string, the hash is zero.  If
 * the argument is contains one byte, then the hash is just the integer value
 * of that byte.  Otherwise, for each byte we bitshift the hash one bit left,
 * then add the value of that byte minus one, multiplied by the value of the
 * following byte.
 */
unsigned int hash_shimmy2(const char *k) {
    if (!k) {
        return 0;
    }
    unsigned int result = 0;
    const size_t len = strlen(k);
    if (len == 0) {
        return 0;
    } else if (len == 1) {
        return k[0];
    }
    for (unsigned int i = 0; i < len - 1; i++) {
        result <<= 1;
        result += k[i+1] * (k[i] - 1);
    }
    return result;
}
