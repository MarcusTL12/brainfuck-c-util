int read_string_delim(int[] s, int delim) {
    int i = 0;
    int c = read_char();
    while (c != delim) {
        s[i] = c;
        c = read_char();
        i = i + 1;
    }

    return i;
}

void write_string(int[] s) {
    int i = 0;
    while (s[i] != 0) {
        write_char(s[i]);
        i = i + 1;
    }
}

void write_string_n(int[] s, int n) {
    int i = 0;
    while (i < n) {
        write_char(s[i]);
        i = i + 1;
    }
}

int divrem(int[] r, int d) {
    int q = 0;

    while (r[0] >= d) {
        r[0] = r[0] - d;
        q = q + 1;
    }

    return q;
}

void write_hex(int n) {
    int chars[] = "0123456789ABCDEF";
    int l[] = {n};
    int h = divrem(l, 16);

    write_char(chars[h]);
    write_char(chars[l[0]]);
}

void write_dec(int n) {
    int buf[] = {n};
    int d = divrem(buf, 100);
    bool written = false;
    if (d != 0) {
        write_char('0' + d);
        written = true;
    }

    d = divrem(buf, 10);
    if (d != 0 || written) {
        write_char('0' + d);
    }

    write_char('0' + buf[0]);
}
