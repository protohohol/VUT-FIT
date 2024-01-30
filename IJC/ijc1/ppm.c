// ppm.c
// Řešení IJC-DU1, příklad a), 22.3.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include "ppm.h"

struct ppm* ppm_read(const char* filename) {
    char* format = strrchr(filename, '.');
    format++;
    if (strcmp(format, "ppm\0")) {
        warning_msg("Wrong file format : %s\n", format);
        return NULL;
    }

    FILE* f;
    if ((f = fopen(filename, "rb")) == NULL) {
        error_exit("Cannot open the file %s\n", filename);
    }

    unsigned x;
    unsigned y;
    char mag_num[3];

    fscanf(f, "%s %u %u 255", mag_num, &x, &y);

    if (strcmp(mag_num, "P6")) {
        warning_msg("Wrong magic number!\n");
        return NULL;
    }

    if ((x * y * 3) > MAX_SIZE_PIC) {
        error_exit("Your file (%s) is too big : %u; Max size : %u", filename, (x * y * 3), MAX_SIZE_PIC);
    }

    struct ppm* image = malloc((x * y * 3) + sizeof(struct ppm));
    if (image == NULL) {
        warning_msg("image : memory allocation error!\n");
    }

    image->xsize = x;
    image->ysize = y;

    unsigned long counter = fread(image->data, 1, (size_t)(image->xsize * image->ysize * 3), f);
    if (counter != (image->xsize * image->ysize * 3)) {
        free(image);
        error_exit("fread error!\n");
    }

    fclose(f);

    return image;
}

void ppm_free(struct ppm* p) {
    free(p);
}
