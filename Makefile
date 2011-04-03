CFLAGS=$(shell pkg-config --cflags cairo) -g -Wall -Werror
LDLIBS=$(shell pkg-config --libs cairo)

all:
	cd packages && sh build.sh
	gcc $(CFLAGS) -o ec ec.c commands.c draw.c elements.c packages.c wires.c \
		board-wires.c board-elements.c board-commands.c packages/*.o $(LDLIBS)

