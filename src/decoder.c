#include <stdint.h>
#include "decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

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