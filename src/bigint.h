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

    while (i < n) {
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
