CFLAGS=$(shell pkg-config --cflags cairo) -g -Wall -Werror
LDLIBS=$(shell pkg-config --libs cairo)


OBJS=ec.o commands.o draw.o elements.o packages.o wires.o

all: ec

ec: $(OBJS)

depend: $(basename $(OBJS))
	echo $?

