CFLAGS = -std=c99 -Wall -Werror -Wvla -g
LDFLAGS = -lm

.PHONY: all clean

all: life kernel bobbie

bobbie: bobbie.c
	gcc $(CFLAGS) bobbie.c -o bobbie $(LDFLAGS)

life: life.c
	gcc $(CFLAGS) life.c -o life $(LDFLAGS)

pgm.o: pgm.c
	gcc $(CFLAGS) pgm.c -c

kernel: kernel.c pgm.o
	gcc $(CFLAGS) kernel.c pgm.o -o kernel $(LDFLAGS)

clean:
	rm -f kernel pgm.o life bobbie
