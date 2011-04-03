#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wires.h"
#include "elements.h"

struct wire {
	char a[32];
	int ap;
	char b[32];
	int bp;
} wires[1024];

int wiren=0;

struct wire *find_wire(char *a,int ap,char *b,int bp) {
	int i;
	for(i=0;i<wiren;i++) {
		struct wire *w=&wires[i];
		if(strcmp(w->a,a)==0 && strcmp(w->b,b)==0 && w->ap==ap && w->bp==bp) return w;
	}
	return 0;
}

struct wire *make_wire(char *a,int ap,char *b,int bp) {
	struct wire *w=wires+wiren++;
	strcpy(w->a,a); strcpy(w->b,b);
	w->ap=bp; w->bp=bp;
	return w;
}

struct wire *wire(unsigned int i) {
	if(i<wiren) return wires+i;
	return 0;
}

char *wire_a(struct wire *w) { return w->a; }
char *wire_b(struct wire *w) { return w->b; }
int wire_ap(struct wire *w) { return w->ap; }
int wire_bp(struct wire *w) { return w->bp; }

