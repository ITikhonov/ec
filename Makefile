CFLAGS=$(shell pkg-config --cflags cairo) -g -Wall -Werror
LDLIBS=$(shell pkg-config --libs cairo)

all:
	gcc $(CFLAGS) -o ec ec.c commands.c draw.c elements.c packages.c wires.c $(LDLIBS)

