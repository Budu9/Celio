#include <stdio.h>
#include <stdint.h>
#include "header.h"
#include "encoder.h"
#include "transformer.h"
#include <stdlib.h>
#include <string.h>

#define WIDTH 1920
#define HEIGHT 1080

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
    if (total_bits <= 0) return ;
    FILE *f = fopen(filename, "wb");

    
    // P6 for standard binary RGB
    // %10d forces width and height to be 10 bytes making the header size predictable
    fprintf(f, "P6\n%10d %10d\n255\n", WIDTH, HEIGHT);
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


int encode_frame_sequence(char *filename, uint8_t *buffer, uint64_t total_bits, uint8_t redundancy) {
    uint32_t bits_per_frame = WIDTH * HEIGHT / redundancy;

    // write first frame (header)
    char header_name[128];
    snprintf(header_name, sizeof(header_name), "%s_%lu.ppm", filename, 0ul);
    uint8_t *temp_header = calloc(1, bits_per_frame);
    memcpy(temp_header, buffer, sizeof(Header));
    write_ppm_frame(header_name, temp_header, bits_per_frame, redundancy);
    free(temp_header);
    buffer += sizeof(Header);

        
    uint64_t payload_bits = total_bits - sizeof(Header) * 8;
    uint64_t num_payload_frames = payload_bits / bits_per_frame;

    for (uint64_t i = 1; i < num_payload_frames + 1; i++) {
        // it is gonna be filename + num for each frame name
        char frame_name[128];
        snprintf(frame_name, sizeof(frame_name), "%s_%lu.ppm", filename, i);

        write_ppm_frame(frame_name, buffer, bits_per_frame, redundancy);
        buffer += (bits_per_frame / 8);
    }

    uint64_t num_frames = num_payload_frames + 1; // +1 for the header frame

    // leftover
    char frame_name[128];
    snprintf(frame_name, sizeof(frame_name), "%s_%lu.ppm", filename, num_frames);

    // we want to write a whole frame
    uint64_t full_frame_bytes = (WIDTH * HEIGHT) / redundancy / 8;
    uint64_t leftover_bits = payload_bits - num_payload_frames * bits_per_frame;

    if (leftover_bits <= 0) return num_frames;

    // celiling division
    uint64_t leftover_bytes = (leftover_bits + 7) / 8;

    // intialise temporary buffer with 0s
    uint8_t *temp = calloc(full_frame_bytes, 1);
    memcpy(temp, buffer, leftover_bytes);

    write_ppm_frame(frame_name, temp, bits_per_frame, redundancy);

    free(temp);

    return ++num_frames;
}