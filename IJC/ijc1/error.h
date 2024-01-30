// error.h
// Řešení IJC-DU1, příklad a), 22.3.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void warning_msg(const char* fmt, ...);
void error_exit(const char* fmt, ...);

#endif
