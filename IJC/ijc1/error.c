// error.c
// Řešení IJC-DU1, příklad a), 22.3.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include "error.h"

void warning_msg(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "ERROR : ");
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

void error_exit(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "ERROR : ");
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(1);
}
