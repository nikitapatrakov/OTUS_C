#include "const.h"
#include "utils.h"

int16_t *getTypeCode(const char* arr)
{
    if (strcmp("cp1251", arr) == 0) return charsets[0];
    else if (strcmp("koi8", arr) == 0) return charsets[1];
    else if (strcmp("iso8859", arr) == 0) return charsets[2];
    return NULL;
}

void writeEncodeSymbol(const int16_t *symbol,const int16_t *typecode, FILE *dst)
{
    if (*symbol == EOF) 
        {   
            return;
        }
    if (*symbol < 128) fputc(*symbol, dst);
    else {
        int16_t encodeSymbol = typecode[*symbol-ASCII_CONST];
        fputc((encodeSymbol>>6) | 0xC0, dst);
        fputc((encodeSymbol & 0x3F) | 0x80, dst);}
}