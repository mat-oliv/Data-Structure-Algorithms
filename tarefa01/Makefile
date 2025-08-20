CFLAGS = -std=c99 -Wall -Werror -Wvla -g
LDFLAGS = -lm

.PHONY: all clean

all: xou swift primos

xou: xou.c
	gcc $(CFLAGS) xou.c -o xou $(LDFLAGS)

swift: swift.c
	gcc $(CFLAGS) swift.c -o swift $(LDFLAGS)

primos: primos.c
	gcc $(CFLAGS) primos.c -o primos $(LDFLAGS)

clean:
	rm -f xou swift primos
