#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>

bool		checkRarjpegFile(FILE *file, off_t fileSize);
void		printCont(FILE *file, off_t fileSize);

#endif // UTILS_H