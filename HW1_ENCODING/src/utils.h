#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*Функция для получения типа кодировки файла*/
int16_t *getTypeCode(const char *arr);

/*Функция записи символа UTF-8*/
void writeEncodeSymbol(const int16_t *symbol,const int16_t *typecode, FILE *dst);

#endif