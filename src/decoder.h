#ifndef ENCODER_H
#define ENCODER_H
#include <stdint.h>

void decode(const char *output_path);
uint64_t read_ppm_frame(char *input_path, uint8_t redundancy, uint8_t *buffer, uint64_t max_bits, uint64_t bit_offset);
void decode_frame_sequence(char *input_path, char *output_path, uint8_t redundancy, uint64_t num_frames); 
#endif