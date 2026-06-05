#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void) {
    FILE *f = fopen("../encode/input.txt", "rb");
    if (!f) {
        perror("File did not open properly");
        return 1;
    }

    // get file size
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
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
            int8_t bit = (byte >> j) & 1;
            printf("%i\n", bit);
        }
    }

    free(buffer);

    return 0;
}