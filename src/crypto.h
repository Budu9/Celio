#ifndef CRYPTO_H
#define CRYPTO_H

#include "header.h"
#include <stdint.h>

uint8_t *generate_key(uint8_t *buffer, char *password);
void generate_key_stream(uint8_t *key, uint8_t *out, uint64_t out_length);
void apply_key_stream(uint8_t *payload, uint8_t *key_stream, uint64_t payload_length);
uint8_t *derive_key(uint8_t salt[32], char *password);
#endif