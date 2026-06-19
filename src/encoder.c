#include <stdio.h>
#include <stdint.h>
#include "header.h"
#include "encoder.h"

void encode(const char *output_path, uint8_t *buffer, uint64_t size) {
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

void write_ppm_frame(char *filename, uint8_t *buffer, uint64_t total_bits, uint8_t redundancy) {
    FILE *f = fopen(filename, "wb");
    int width = 1920;
    int height = (total_bits*redundancy / width) + 1;
    
    // P6 for standard binary RGB
    // %10d forces width and height to be 10 bytes making the header size predictable
    fprintf(f, "P6\n%10d %10d\n255\n", width, height);
    for(uint64_t i = 0; i < total_bits; i++) {
        uint8_t bit = get_bit_at(buffer, i);


        uint8_t colour = (bit == 1) ? 0 : 255;

        // horizontal redundancy for now
        // might change for nxn block redundancy later which apparently works better against video compression
        // but should be fine for now
        for (int j = 0; j < redundancy; j++) {
            // 3 times for RGB
            fputc(colour, f);
            fputc(colour, f);
            fputc(colour, f);
        }

    }
    fclose(f);
}