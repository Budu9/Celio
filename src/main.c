#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "header.h"
#include "encoder.h"
#include <string.h>
#include "transformer.h"
#include "decoder.h"

int main(int argc, char *argv[]) {
    char *path = "input.txt";
    FILE *f = fopen(path, "rb");
    if (!f) {
        perror("File did not open properly");
        return 1;
    }

    // get file size
    fseek(f, 0, SEEK_END);
    uint64_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // allocate buffer size dynamically
    uint8_t *buffer = malloc(size);

    if (buffer == NULL) {
        fprintf(stderr, "Memory alloc failed");
        fclose(f);
        return 1;
    }

    fread(buffer, 1, size, f);
    fclose(f);

    Header header = create_header(path, size);

    // concatanate payload with header into single buffer
    uint64_t total_size = sizeof(header) + size;

    // allocate mem for total buffer
    uint8_t *total_buffer = malloc(total_size);

    // copy header data first
    memcpy(total_buffer, &header, sizeof(Header));

    // then payload
    memcpy(total_buffer + sizeof(Header), buffer, size);

    for (uint64_t i = 0; i < total_size*8; i++) {
        printf("%u\n", get_bit_at(total_buffer, i));
    }

    encode("outputraw", total_buffer, total_size);

    free(buffer);

    decode("outputraw");

    return 0;
}