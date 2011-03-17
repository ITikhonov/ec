#include "wires.h"

struct wire {
	struct element *a;
	int ap;
	struct element *b;
	int bp;
} wires[1024];

int wiren=0;

void make_wire(struct element *a,int ap,struct element *b,int bp) {
	struct wire *w=wires+wiren++;
	w->a=a; w->ap=ap;
	w->b=b; w->bp=bp;
}

struct wire *wire(unsigned int i) {
	if(i<wiren) return wires+i;
	return 0;
}

struct element *wire_a(struct wire *w,int *p) { *p=w->ap; return w->a; }
struct element *wire_b(struct wire *w,int *p) { *p=w->bp; return w->b; }

