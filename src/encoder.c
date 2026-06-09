#include <stdio.h>
#include <stdint.h>
#include "header.h"
#include "encoder.h"

void write_file(const char *output_path, uint8_t *buffer, uint64_t size) {
    FILE *out = fopen(output_path, "wb");
    if (!out) {
        perror("Failed to open output file");
        return;
    }

    // write whole total_buffer once
    fwrite(buffer, 1, size, out); 

    fclose(out);
    printf("Encoded to %s\n", output_path);
}