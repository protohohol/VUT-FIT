// tail.c
// Řešení IJC-DU2, příklad a), 20.4.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void printl(FILE* f, long amount) {
    long n_l = 0; //number of lines
    unsigned long n_c = 0; //number of chars
    int l_p[amount + 1]; //list of pointers
    int counter = 0;
    int l_counter = 0; //local char counter
    bool b_f = 1;


    for (char c = getc(f); !feof(f); c = getc(f)) {
        n_c++;
        l_counter++;
        if (c == '\n') {
            if (b_f) {
                l_p[counter] = n_c - l_counter;
                if (counter == amount - 1) {
                    b_f = 0;
                }
            } else {
                for (long i = 0; i < amount - 1; i++) {
                    l_p[i] = l_p[i + 1];
                }
                l_p[counter] = n_c - l_counter;
            }

            if (counter < amount-1) {
                counter++;
            }
            
            l_counter = 0;
            n_l++;
        }
    }
    if (counter > n_l) {
        counter = n_l;
    }
    if (counter < amount - 1 && counter != 0) {
        counter--;
    }
    
    for (long i = 0; i <= counter; i++) {
        l_counter = 0;
        fseek(f, l_p[i], SEEK_SET);
        for (char c = getc(f); c != '\n'; c = getc(f)) {
            l_counter++;
            if (l_counter > 4095) {
                fprintf(stderr, "Your line is too long!\n");
                break;
            } else {
                printf("%c", c);
            }
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    (void) argc;
    (void) argv;

    FILE* f = NULL;
    long a_l = 10; //amount of lines

    if (argc == 1) {
        f = stdin;
        if (f == NULL) {
            fprintf(stderr, "Error reading from stdin!\n");
        } else {
            printl(f, a_l);
            return 0;
        }
    }
    if (argc > 1) {
        if (!strcmp(argv[1], "-n")) {
            if (atol(argv[2]) < 0) {
                fprintf(stderr, "Wrong number of strings!\n");
                return 1;
            } else if (atol(argv[2]) == 0) {
                fprintf(stderr, "illegal offset -- %s\n", argv[2]);
                return 1;
            } else {
                a_l = atol(argv[2]);
            }

            if (argc == 4) {
                f = fopen(argv[3] , "r");
                if (f == NULL) {
                    fprintf(stderr, "Cannot read a file!\n");
                    return 1;
                } else {
                    printl(f, a_l);
                    return 0;
                }
            } else if (argc == 3) {
                f = stdin;
                if (f == NULL) {
                    fprintf(stderr, "Error reading from stdin!\n");
                } else {
                    printl(f, a_l);
                    return 0;
                }
            } else {
                fprintf(stderr, "Wrong amount of arguments!\n");
            }
        } else {
            f = fopen(argv[1] , "r");
            if (f == NULL) {
                fprintf(stderr, "Cannot read a file\n");
                return 1;
            } else {
                printl(f, a_l);
                return 0;
            }
        }
    }

    return 0;
}
