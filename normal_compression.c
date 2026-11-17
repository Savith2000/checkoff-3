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
        int count = in[i]; /* raw number, no - '0' needed */
        unsigned char c = in[i + 1];

        for (int k = 0; k < count; k++) out[o++] = c;
    }
    return o;
}

int main(void) {
    unsigned char input[300];
    unsigned char encoded[600];
    unsigned char decoded[300];

    memset(input, 'A', 300);

    size_t enc_len = rle_encode(input, 300, encoded);
    size_t dec_len = rle_decode(encoded, enc_len, decoded);

    printf("encoded %zu bytes:", enc_len);
    for (size_t i = 0; i < enc_len; i++) printf(" %d", encoded[i]);
    printf("\n");

    printf("decoded %zu bytes, matches original: %s\n", dec_len,
           memcmp(input, decoded, 300) == 0 ? "yes" : "no");
    return 0;
}