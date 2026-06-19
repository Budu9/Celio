#include <stdint.h>
#include "decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "transformer.h"

void decode(const char *output_path) {
    FILE *f = fopen(output_path, "rb");
    if (!f) return;

    Header header;
    fread(&header, sizeof(Header), 1, f);

    uint8_t *payload = malloc(header.file_size);
    

    fread(payload, header.file_size, 1, f);
    fclose(f);


    FILE *out = fopen("output_same_as_input", "wb");
    if (out) {
        fwrite(payload, header.file_size, 1, out);
        fclose(out);
    }

    free(payload);
    printf("Decoded to %s\n", output_path);
}

void read_ppm_frame(char *input_path, char *output_path) {
    FILE *f = fopen(input_path, "rb");

    // 29 is the ppm header size (view encoder comment)
    fseek(f, 29, SEEK_SET); // discard ppm header

    Header header;

    for (int i = 0; i < sizeof(header)*8; i++) {
        // maybe implement different colours in the future somehow?
        uint8_t r = fgetc(f);
        uint8_t g = fgetc(f);
        uint8_t b = fgetc(f);

        uint8_t bit = r == 0 ? 1 : 0;

        set_bit_at(&header, i, bit);
    }

    uint8_t *payload = malloc(header.file_size);

    for (int i = 0; i < header.file_size*8; i++) {
        uint8_t r = fgetc(f);
        uint8_t g = fgetc(f);
        uint8_t b = fgetc(f);

        uint8_t bit = r == 0 ? 1 : 0;

        set_bit_at(payload, i, bit);
    }

    fclose(f);

    FILE *out = fopen("output", "wb");
    if (out) {
        fwrite(payload, header.file_size, 1, out);
        fclose(out);
    }

    free(payload);
    printf("Decoded to %s\n", output_path);
}
