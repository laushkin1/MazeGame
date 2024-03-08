CC=gcc
CFLAGS=-std=c11 -Wall -Werror
LDLIBS=-lm -lncurses
OUTPUT=$@

%: %.c
	$(CC) $(CFLAGS) $@.c $(LDLIBS) -o $(OUTPUT)

clean:
	rm program score.txt 
