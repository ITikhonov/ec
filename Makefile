CFLAGS=$(shell pkg-config --cflags cairo)
LDLIBS=$(shell pkg-config --libs cairo)

all: ec

