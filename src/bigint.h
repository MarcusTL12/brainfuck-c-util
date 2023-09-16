#ifndef __bigint__
#define __bigint__

#include "memory.h"
#include "util.h"

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
    if (b > 127 && n > 1) {
        a[1] = a[1] + 1;
        b = b - 128;
    }

    a[0] = a[0] + b;
    bool carry = a[0] > 127;
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

// Subtract single unsigned byte to n byte bigint a
void bigint_sub_small(int[] a, int b, int n) {
    if (b > 127 && n > 1) {
        a[1] = a[1] - 1;
        b = b - 128;
    }

    b = -b - 128;

    a[0] = a[0] + b;
    bool carry = a[0] > 127;
    if (carry) {
        a[0] = a[0] - 128;
    }

    int i = 1;

    while (i < n) {
        a[i] = a[i] + 127;
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

// Leave the 2s complement negative of a in a
void bigint_negate(int[] a, int n) {
    int i = 0;

    while (i < n) {
        a[i] = -a[i] + 127;

        i = i + 1;
    }

    bigint_add_small(a, 1, n);
}

// Compute the integer division of the 2-byte int a with the 1-byte int d
// Returns a % d, and leaves a / d in a
int divrem_int2(int[] a, int d) {
    // Make a contain r0 = ((a1 % d) b + a0)
    // This is redundand when a1 < d
    int buf[] = {a[1]};
    int ans1 = divrem(buf, d);
    a[1] = buf[0];

    int ans0 = 0;

    while (a[1] != 0 || a[0] >= d) {
        ans0 = ans0 + 1;

        bigint_sub_small(a, d, 2);
    }

    int r = a[0];
    a[0] = ans0;
    a[1] = ans1;

    return r;
}

// Compute the integer division of the 2-byte int a with the 1-byte int d
// Returns a / d, and leaves a % d in a[0]
// This is an optimized version that assumes that a[1] < d
int divrem_int2_reduced(int[] a, int d) {
    // Make a contain r0 = ((a1 % d) b + a0)
    // This is redundand when a1 < d
    int ans = 0;

    while (a[1] != 0 || a[0] >= d) {
        ans = ans + 1;

        bigint_sub_small(a, d, 2);
    }

    return ans;
}

void bigint_write(int[] a, int n) {}

#endif
