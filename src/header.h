#ifndef HEADER_H
#define HEADER_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint32_t magic_number;
    uint64_t file_size;
    uint16_t name_len;
    char filename[260];
} Header;

Header create_header(char *path, uint64_t size);

#endif