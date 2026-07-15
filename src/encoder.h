#ifndef ENCODER_H
#define ENCODER_H

#include "header.h"
#include <stdint.h>

void encode(const char *output_path, uint8_t *buffer, uint64_t size);
void write_ppm_frame(char *filename, uint8_t *buffer, uint64_t total_bits, uint8_t redundancy);
int encode_frame_sequence(char *filename, uint8_t *buffer, uint64_t total_bits, uint8_t redundancy);
#endif