# steg-decode.c
# Řešení IJC-DU1, příklad a), 22.3.2022
# Autor: Vsevolod Tiemnohorov, FIT


LC_ALL=cs_CZ.utf8
CFLAGS=-g -std=c11 -pedantic -Wall -Wextra
CC=gcc

primes=primes.o eratosthenes.o bitset.o error.o
primes-i=primes-i.o eratosthenes-i.o bitset-i.o

all: stack_set primes primes-i steg-decode

stack_set:
	ulimit -s 40000

$(primes): *.c
	$(CC) $(CFLAGS) -c $^

primes-i.o: primes.c
	$(CC) $(CFLAGS) -DUSE_INLINE -c primes.c -o primes-i.o

bitset-i.o: bitset.c
	$(CC) $(CFLAGS) -DUSE_INLINE -c bitset.c -o bitset-i.o
	
eratosthenes-i.o: eratosthenes.c
	$(CC) $(CFLAGS) -DUSE_INLINE -c eratosthenes.c -o eratosthenes-i.o

primes: primes.o eratosthenes.o bitset.o error.o
	$(CC) $(CFLAGS) $^ -o $@ -lm

primes-i: primes-i.o eratosthenes-i.o bitset-i.o error.o
	$(CC) $(CFLAGS) $^ -o $@ -O0 -lm

steg-decode: steg-decode.o ppm.o eratosthenes.o bitset.o error.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

run: all
	./primes
	./primes-i

clean:
	rm -f *.o primes primes-i steg-decode