#include <stdint.h>
#include <stdio.h>
#include "header.h"
#include <string.h>
#include <stdlib.h>
#include <sodium.h>

void apply_key_stream(uint8_t *payload, uint8_t *key_stream, uint64_t payload_length) {
    for (uint64_t i = 0; i < payload_length; i++) {
        *(payload+i) ^= *(key_stream+i);
    }
}

void generate_key_stream(uint8_t *key, uint8_t *out, uint64_t out_length) {
    uint64_t counter = 0;
    uint64_t offset = 0;
    
    while (offset < out_length) {
        // block is key + counter
        uint8_t block[crypto_hash_sha256_BYTES + sizeof(counter)];
        memcpy(block, key, crypto_hash_sha256_BYTES);
        memcpy(block+crypto_hash_sha256_BYTES, &counter, sizeof(counter));

        // hash block
        uint8_t hashed_block[crypto_hash_sha256_BYTES];
        crypto_hash_sha256(hashed_block, block, sizeof(block));

        // each chunk is the min of the hash length and the remaining
        uint64_t chunk = (out_length - offset) < crypto_hash_sha256_BYTES ? (out_length-offset) : crypto_hash_sha256_BYTES;
        memcpy(out + offset, hashed_block, chunk);
        counter++;
        offset += chunk;
    }
}

uint8_t *generate_key(uint8_t *buffer, char *password) {
    
    Header *header = (Header *)buffer;

    // generate salt
    FILE *urandom = fopen("/dev/urandom", "rb");
    if (!urandom) { perror("failed to open /dev/urandom"); return NULL; }
    fread(header->salt, 1, sizeof(header->salt), urandom);
    fclose(urandom);

    // generate key as sha256 of password + salt
    size_t pw_len = strlen(password);
    size_t concat_len = pw_len + sizeof(header->salt);
    uint8_t *concat = malloc(concat_len);
    memcpy(concat, password, pw_len);
    memcpy(concat + pw_len, header->salt, sizeof(header->salt));

    uint8_t *key = malloc(crypto_hash_sha256_BYTES);
    crypto_hash_sha256(key, concat, concat_len);
    free(concat);
    return key;
}

uint8_t *derive_key(uint8_t salt[32], char *password) {
    size_t pw_len = strlen(password);
    size_t concat_len = pw_len + 32;
    uint8_t *concat = malloc(concat_len);
    memcpy(concat, password, pw_len);
    memcpy(concat + pw_len, salt, 32);
    uint8_t *key = malloc(crypto_hash_sha256_BYTES);
    crypto_hash_sha256(key, concat, concat_len);
    free(concat);
    return key;
}

