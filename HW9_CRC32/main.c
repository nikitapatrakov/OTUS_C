#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "const.h"

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

uint32_t
 crc32(const void *buf, size_t size) {
    const uint8_t *p = buf;
    uint32_t crc;

    crc = ~0U;
    while (size--)
        crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);
    return crc ^ ~0U;
 }

int main(int argc, char **argv)
{   
    int fb;
    struct stat statbuf;
    void *addr;
    if (argc < 2) {
        perror("Missing filename argument");
        return EXIT_FAILURE;
    }
    fb = open(argv[1], O_RDONLY);
    if (fb < 0)
        handle_error("open");

    if (fstat(fb, &statbuf) < 0)
        handle_error("fstat");
    
    addr = mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, fb, 0);
    if (addr == MAP_FAILED)
        handle_error("mmap");
    uint32_t crc = crc32((const void *)addr, statbuf.st_size);
    printf ("CRC32 = %x\n", crc);

    return EXIT_SUCCESS;  
}