CFLAGS = -std=c99 -Wall -Werror -Wvla -g
LDFLAGS = -lm

.PHONY: all clean

all: drama cpf primos

drama: drama.c
	gcc $(CFLAGS) drama.c -o drama $(LDFLAGS)

cpf: cpf.c
	gcc $(CFLAGS) cpf.c -o cpf $(LDFLAGS)

primos: primos.c
	gcc $(CFLAGS) primos.c -o primos $(LDFLAGS)

clean:
	rm -f drama cpf primos
