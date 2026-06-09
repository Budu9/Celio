#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "header.h"
#include "encoder.h"

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

    for (size_t i = 0; i < (size_t)size; i++) {
        // uint8_t because exactly 1 byte
        uint8_t byte = buffer[i];
        for (int j = 0; j < 8; j++) {
            uint8_t bit = (byte >> j) & 1;
            printf("%i\n", bit);
        }
    }

    Header header;
    header = create_header(path, size);
    printf("Magic Number: %X\n", header.magic_number);
    printf("Filename: %s\n", header.filename);
    printf("Size: %llu\n", header.file_size);

    write_file("output", header, buffer, size);

    free(buffer);

    return 0;
}