#include <string.h>

/*
 * Compute an unsigned long integer hash from a NUL-terminated byte array.
 *
 * If the argument is a NULL pointer, or an empty string, the hash is zero.  If
 * the argument contains one byte, then the hash is just the integer value
 * of that byte.  Otherwise, for each byte we bitshift the hash one bit left,
 * then add the value of that byte minus one, multiplied by the value of the
 * following byte.
 */
unsigned long hash_shimmy2(const char *bytes) {
    if (!bytes) {
        return 0;
    }
    unsigned long result = 0;
    const size_t len = strlen(bytes);
    if (len == 0) {
        return 0;
    } else if (len == 1) {
        return bytes[0];
    }
    for (unsigned int i = 0; i < len - 1; i++) {
        result <<= 1;
        result += bytes[i+1] * (bytes[i] - 1);
    }
    return result;
}
