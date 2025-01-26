#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>

bool		checkRarjpegFile(FILE *file, __off_t fileSize);
void		printCont(FILE *file, __off_t fileSize);

#endif // UTILS_H