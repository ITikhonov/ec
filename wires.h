struct element;

struct wire *make_wire(struct element *,int,struct element *,int);

struct wire *wire(unsigned int i);

struct element *wire_a(struct wire *,int *);
struct element *wire_b(struct wire *,int *);

struct wire *pick_wire_corner(int x,int y,int *no,int nth);

int wire_corner(struct wire *,int,int*x,int*y);

#include <stdio.h>

void wires_load(FILE *);

