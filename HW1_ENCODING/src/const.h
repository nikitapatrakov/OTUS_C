#ifndef CONST_H
#define CONST_H

#include <stdint.h>

#define ASCII_CONST 128 /*Констанстное значение дублирующих символов*/

/*Символы таблиц CP1251*/
extern int16_t cp1251[];

/*Символы таблиц ISO8859*/
extern int16_t iso8859[];

/*Символы таблиц koi8*/
extern int16_t koi8[];

extern int16_t* charsets[];

#endif