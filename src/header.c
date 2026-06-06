#include "header.h"
#include <sys/stat.h>
#include <string.h>

Header create_header(char *path, uint64_t size) {
    Header header;
    
    header.magic_number = 0x4F4C4543; // CELO
    header.file_size = size;
    strncpy(header.filename, path, 259);
    header.name_len = (uint16_t)strlen(header.filename);

    return header;
}