#ifndef __bigint__
#define __bigint__

#include "memory.h"

// Structure of bigint:
// Each byte is treated as one 7-bit part of the number
// using the 8th bit as the carry bit.

// Add two n-byte bigints and store result in a
void bigint_add(int[] a, int[] b, int n) {
    int i = 0;

    bool carry = false;

    while (i < n) {
        a[i] = a[i] + b[i];
        if (carry) {
            a[i] = a[i] + 1;
        }

        carry = a[i] > 127;
        if (carry) {
            a[i] = a[i] - 128;
        }

        i = i + 1;
    }
}

// Add single unsigned byte (full 8 bit supported) to n byte bigint a
void bigint_add_small(int[] a, int b, int n) {
    bool carry = b > 127;
    a[0] = a[0] + b;
    if (carry) {
        a[0] = a[0] + 1;
    }
    carry = a[0] > 127;
    if (carry) {
        a[0] = a[0] - 128;
    }

    int i = 1;
    bool done = false;

    while (!done && i < n) {
        if (carry) {
            a[i] = a[i] + 1;
        }

        carry = a[i] > 127;
        if (carry) {
            a[i] = a[i] - 128;
        } else {
            done = true;
        }

        i = i + 1;
    }
}

// Parse string of length len into n-byte bigint a.
// An n-byte bigint b is needed for buffer space.
void bigint_parse(int[] str, int len, int[] a, int[] b, int n) {
    int i = 0;

    while (i < len) {
        int d = str[i] - '0';

        memcpy(b, a, n);

        int j = 0;
        while (j < 9) {
            bigint_add(a, b, n);
            j = j + 1;
        }

        bigint_add_small(a, d, n);

        i = i + 1;
    }
}

#endif
