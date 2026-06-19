#ifndef ENCODER_H
#define ENCODER_H
#include <stdint.h>

void decode(const char *output_path);
void read_ppm_frame(char *input_path, char *output_path, uint8_t redundancy);
#endif