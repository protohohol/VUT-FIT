CFLAGS=-std=c11 -pedantic -Wall -Wextra
CC=gcc

library=htab_init.o htab_bucket_count.o htab_clear.o htab_erase.o htab_find.o htab_for_each.o htab_free.o htab_hash_function.o htab_lookup_add.o htab_resize.o htab_size.o

all: libhtab.a libhtab.so wordcount wordcount-dynamic tail

$(library): *.c
	$(CC) $(CFLAGS) -fPIC -c $^

libhtab.a: $(library)
	ar -rcs libhtab.a $(library)

libhtab.so: $(library)
	$(CC) $(CFLAGS) -shared -fPIC -o libhtab.so $(library)

wordcount: wordcount.o io.o libhtab.a
	$(CC) $(CFLAGS) -o wordcount wordcount.o io.o -L . libhtab.a

wordcount-dynamic: wordcount.o io.o libhtab.so
	$(CC) $(CFLAGS) -o wordcount-dynamic wordcount.o io.o libhtab.so

tail: tail.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o wordcount wordcount-dynamic tail