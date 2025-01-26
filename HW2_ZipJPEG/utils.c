#include "utils.h"
#include "zipinfo.h"


bool checkRarjpegFile(FILE *file, __off_t fileSize)
{
    fseek(file, fileSize, SEEK_SET);
}