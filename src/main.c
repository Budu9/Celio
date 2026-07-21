#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "header.h"
#include "encoder.h"
#include <string.h>
#include "decoder.h"
#include "crypto.h"
#include <sodium.h>

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

    if (sodium_init() < 0) {
        printf("sodium error\n");
        return -1;
    }

    uint8_t *key = generate_key(total_buffer, "password");
    uint8_t *out = malloc(size);
    generate_key_stream(key, out, size);
    apply_key_stream(total_buffer+sizeof(Header), out, size);
    ((Header *)total_buffer)->encrypted = 1;
    free(out);

    uint64_t num_frames = encode_frame_sequence("ppm_file", total_buffer, total_size*8, 4, size);
    printf("%lu frames created\n", num_frames);
    
    

    decode_frame_sequence("ppm_file", "output", 4, num_frames);


    free(buffer);

    return 0;
}