#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "header.h"
#include "encoder.h"
#include <string.h>

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

    Header header;
    header = create_header(path, size);
    printf("Magic Number: %X\n", header.magic_number);
    printf("Filename: %s\n", header.filename);
    printf("Size: %llu\n", header.file_size);

    // concatanate payload with header into single buffer
    uint64_t total_size = sizeof(header) + size;

    // allocate mem for total buffer
    uint8_t *total_buffer = malloc(total_size);

    // copy header data first
    memcpy(total_buffer, &header, sizeof(Header));

    // then payload
    memcpy(total_buffer + sizeof(Header), buffer, size);


    write_file("output", total_buffer, total_size);

    free(buffer);

    return 0;
}