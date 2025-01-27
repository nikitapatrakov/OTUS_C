#include "utils.h"
#include "zipinfo.h"


bool checkRarjpegFile(FILE *file, off_t fileSize)
{   
    off_t position = fileSize - sizeof(EOCD);
    uint32_t sig=0;
    while(position)
    {   
        fseek(file, position, SEEK_SET);
        fread(&sig, sizeof(EOCD), 1, file);
        if(sig==EOCD) return true;
        position--;
    }
    return false;
}

void printCont(FILE *file, off_t fileSize)
{
    printf("YEP");
}