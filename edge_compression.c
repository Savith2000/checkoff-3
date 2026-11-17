#include <stdio.h>
#include <string.h>

size_t rle_encode(const unsigned char* in, size_t n, unsigned char* out) {
    size_t i = 0, o = 0;

    while (i < n) {
        unsigned char c = in[i];
        int count = 1;

        while (i + count < n && in[i + count] == c && count < 255) count++;

        out[o++] = (unsigned char)count;
        out[o++] = c;
        i += count;
    }
    return o;
}

size_t rle_decode(const unsigned char* in, size_t n, unsigned char* out) {
    size_t o = 0;

    for (size_t i = 0; i + 1 < n; i += 2) {
        int count = in[i];
        unsigned char c = in[i + 1];

        for (int k = 0; k < count; k++) out[o++] = c;
    }
    return o;
}

#define FLAG_RAW 0
#define FLAG_RLE 1

size_t compress(const unsigned char* in, size_t n, unsigned char* out) {
    size_t enc_len = rle_encode(in, n, out + 1);

    if (enc_len < n) {
        out[0] = FLAG_RLE;
        return 1 + enc_len;
    } else {
        out[0] = FLAG_RAW;
        memcpy(out + 1, in, n);
        return 1 + n;
    }
}

size_t decompress(const unsigned char* in, size_t n, unsigned char* out) {
    if (n == 0) return 0;

    if (in[0] == FLAG_RLE) {
        return rle_decode(in + 1, n - 1, out);
    } else {
        memcpy(out, in + 1, n - 1);
        return n - 1;
    }
}

int main(void) {
    const char* examples[3] = {"WWWWWWWWWWWWBWWWWWWWWWWWWBBB", "AAABBBCCCD",
                               "HELLO"};

    for (int e = 0; e < 3; e++) {
        const char* text = examples[e];
        size_t n = strlen(text);
        unsigned char compressed[100];
        unsigned char restored[100];

        size_t c_len = compress((const unsigned char*)text, n, compressed);
        size_t r_len = decompress(compressed, c_len, restored);

        printf("input: %s (%zu bytes)\n", text, n);
        printf("compressed: %zu bytes, flag = %d\n", c_len, compressed[0]);
        printf("matches original: %s\n\n",
               r_len == n && memcmp(text, restored, n) == 0 ? "yes" : "no");
    }
    return 0;
}