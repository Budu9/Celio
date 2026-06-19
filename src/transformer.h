#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <stdint.h>

uint8_t get_bit_at(uint8_t *buffer, uint64_t bit_index);
void set_bit_at(uint8_t *buffer, uint64_t bit_index, uint8_t new_value);

#endif