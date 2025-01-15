#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "const.h"
#include "utils.h"


int main(int argc, char *argv[])
{   
    if (argc != 4)
    {
        printf("Example: <inputfile> <Encoding> <outfile>");
        exit(0);
    }

    int16_t *typecode = getTypeCode(argv[2]);
    if (typecode == NULL)
    {
        printf("Encoding options: \"cp1251\", \"koi8\", \"iso8859\"\n");
        exit(0);
    }

    FILE *src = fopen(argv[1], "rb");
    if (src == NULL)
    {
        perror("Error");
        exit(0);
    }

    FILE *dst = fopen(argv[3], "wb");
    if (dst == NULL)
    {
        perror("Error");
        exit(0);
    }

    while (!feof(src)){
        int16_t symbol = getc(src);
        writeEncodeSymbol(&symbol,typecode,dst);}
    return 0;
}