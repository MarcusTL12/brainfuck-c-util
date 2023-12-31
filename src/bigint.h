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

// Divide n-byte integer a by 7 bit unsigned int b
// Quotient is stored in a, and the remainder r is returned
int bigint_divrem_small(int[] a, int d, int n) {
    int i = n - 1;

    int r[] = {0, 0};

    while (i != 255) {
        r[0] = a[i];

        int q = divrem_int2_reduced(r, d);

        a[i] = q;
        r[1] = r[0];

        i = i - 1;
    }

    return r[1];
}

bool bigint_eq_small(int[] a, int b, int n) {
    bool iseq = a[0] == b;

    int i = 1;
    while (iseq && i < n) {
        iseq = iseq && a[i] == 0;
        i = i + 1;
    }

    return iseq;
}

void bigint_write_consume(int[] a, int[] buf, int n) {
    int nd = 0;

    bool iszero = bigint_eq_small(a, 0, n);

    while (!iszero) {
        buf[nd] = bigint_divrem_small(a, 10, n);
        nd = nd + 1;
        iszero = bigint_eq_small(a, 0, n);
    }

    int i = nd - 1;

    while (i != 255) {
        write_char('0' + buf[i]);
        i = i - 1;
    }

    if (nd == 0) {
        write_char('0');
    }
}

void bigint_write(int[] a, int[] buf, int[] digitbuf, int n) {
    memcpy(buf, a, n);
    bigint_write_consume(buf, digitbuf, n);
}

// Compare unsigned bigints.
// Return integer with same sign as a - b and absolute value equal to 1.
// So:
//  0 if a == b
//  1 if a > b
// -1 if a < b
int bigint_cmp(int[] a, int[] b, int n) {
    int i = n - 1;

    int ans = 0;

    while (ans == 0 && i != -1) {
        if (a[i] > b[i]) {
            ans = 1;
        } else {
            if (a[i] < b[i]) {
                ans = -1;
            }
        }

        i = i - 1;
    }

    return ans;
}

#endif
