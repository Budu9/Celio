#include <stdint.h>
#include "decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "transformer.h"
#include "crypto.h"
#include <string.h>

uint64_t read_ppm_frame(char *input_path, uint8_t redundancy, uint8_t *buffer, uint64_t max_bits, uint64_t bit_offset) {

    FILE *f = fopen(input_path, "rb");

    // 29 is the ppm header size (view encoder comment)
    fseek(f, 29, SEEK_SET); // discard ppm header

    uint64_t bit_counter = 0;


    for (uint64_t i = 0; i < max_bits; i++) {
        // maybe implement different colours in the future somehow?

        if (i >= max_bits) return 0;

        // you could check against -1 (which becomes 255) but then 255 could be actual data so check before
        int r_raw = fgetc(f);
        if (r_raw == EOF) { fclose(f); return bit_counter; }
        uint8_t r = (uint8_t)r_raw;
        
        uint8_t g = fgetc(f);
        uint8_t b = fgetc(f);


        // TODO: actually check for errors here
        // and/or hamming codes actually

        for (int j = 0; j < redundancy - 1; j++) {
            r = fgetc(f);
            g = fgetc(f);
            b = fgetc(f);
        }

        uint8_t bit = r == 0 ? 1 : 0;

        set_bit_at(buffer, bit_offset + i, bit);
        bit_counter += 1;
    }


    fclose(f);
    return bit_counter;
}

void decode_frame_sequence(char *input_path, char *output_path, uint8_t redundancy, uint64_t num_frames) {
    // input_path is the prefix eg. prefix_0, prefix_1 ...

    uint64_t frame_counter = 0;

    Header header;

    // headers and targets in bits
    uint64_t header_counter = 0;
    uint64_t header_target = sizeof(header) * 8;

    uint8_t *payload = NULL;
    
    uint64_t payload_counter = 0;
    uint64_t payload_target = 0;
    

    while (header_counter < header_target && frame_counter < num_frames) {
        char frame_name[128];
        snprintf(frame_name, sizeof(frame_name), "%s_%lu.ppm", input_path, frame_counter);
        printf("%s\n", frame_name); 

        printf("to increase: %lu\n", header_target-header_counter);

        // cast header such that it advances by a bit with each offset
        uint64_t increase = read_ppm_frame(frame_name, redundancy, (uint8_t*)&header, header_target - header_counter, header_counter);
        printf("increase: %lu\n", increase);
        header_counter += increase;
        frame_counter++;
    }

    payload_target = header.file_size * 8;
    payload = malloc(payload_target);
    printf("payload of size %lu\n", payload_target/8);
    
    while (payload_counter < payload_target && frame_counter < num_frames) {
       char frame_name[128];
       snprintf(frame_name, sizeof(frame_name), "%s_%lu.ppm", input_path, frame_counter);

       uint64_t increase = read_ppm_frame(frame_name, redundancy, payload, payload_target - payload_counter, payload_counter);
       payload_counter += increase;
       frame_counter++;
    }
    printf("payload_counter=%lu payload_target=%lu\n", payload_counter, payload_target);

    uint8_t *total_buffer = malloc(sizeof(Header) + header.file_size);
    memcpy(total_buffer, &header, sizeof(Header));
    memcpy(total_buffer + sizeof(Header), payload, header.file_size);

    if (header.encrypted == 1) {
        uint8_t *key = derive_key(header.salt, "password");
        uint8_t *out = malloc(header.file_size);
        generate_key_stream(key, out, header.file_size);
        apply_key_stream(total_buffer+sizeof(Header), out, header.file_size);
        free(out);
    }

    FILE *out = fopen("output", "wb");
    if (out) {
        fwrite(total_buffer + sizeof(Header), header.file_size, 1, out);
        fclose(out);
    }
    
    free(payload);
    printf("Decoded to %s\n", output_path);
    
}