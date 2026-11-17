#include <stdio.h>

void rle_encode(const char* in, char* out) {
    int i = 0;
    int o = 0;

    while (in[i] != '\0') {
        char c = in[i];
        int count = 1;

        while (in[i + count] == c && count < 9) count++;

        out[o++] = '0' + count;
        out[o++] = c;
        i += count;
    }
    out[o] = '\0';
}

void rle_decode(const char* in, char* out) {
    int o = 0;

    for (int i = 0; in[i] != '\0'; i += 2) {
        int count = in[i] - '0';
        char c = in[i + 1];

        for (int k = 0; k < count; k++) out[o++] = c;
    }
    out[o] = '\0';
}

int main(void) {
    char encoded[100], decoded[100];

    rle_encode("AAAAAB", encoded);
    rle_decode(encoded, decoded);

    printf("encoded: %s\n", encoded);
    printf("decoded: %s\n", decoded);
    return 0;
}