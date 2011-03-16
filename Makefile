CFLAGS=$(shell pkg-config --cflags cairo) -g -Wall -Werror
LDLIBS=$(shell pkg-config --libs cairo)

all: ec

ec: ec.o commands.o draw.o elements.o

