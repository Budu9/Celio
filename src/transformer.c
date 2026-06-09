#include "transformer.h"

uint8_t get_bit_at(uint8_t *buffer, uint64_t bit_index) {

    // so index for 10 is 10/8=1 so byte index 1
    uint64_t byte_index = bit_index / 8;

    // so 10 would give 2
    uint8_t bit_offset = bit_index % 8;

    // the bit offset is counter from the right because little endian
    // the single 1 is essentially 00000001
    return (*(buffer+byte_index) >> bit_offset) & 1;

}