#ifndef ENCODER_H
#define ENCODER_H

#include "header.h"
#include <stdint.h>

void write_file(const char *output_path, Header header, uint8_t *buffer, uint64_t size);

#endif