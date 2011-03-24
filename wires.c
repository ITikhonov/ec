#include <stdlib.h>

#include "wires.h"
#include "elements.h"

struct corner { int x,y; };

struct wire {
	struct element *a;
	int ap;
	struct element *b;
	int bp;

	struct corner *corners;
	int cn;
} wires[1024];

int wiren=0;

struct wire *make_wire(struct element *a,int ap,struct element *b,int bp) {
	struct wire *w=wires+wiren++;
	w->a=a; w->ap=ap;
	w->b=b; w->bp=bp;
	w->corners=0;
	w->cn=0;
	return w;
}

struct corner *wire_add_corner(struct wire *w,int segmentno,int x,int y) {
	int cn=w->cn++;
	w->corners=realloc(w->corners,w->cn*sizeof(struct corner));

	int i;
	for(i=segmentno;i<cn;i++) {
		w->corners[i+1]=w->corners[i];
	}
	w->corners[segmentno].x=x;
	w->corners[segmentno].y=y;
	return &w->corners[segmentno];
}

int wire_corner(struct wire *w,int i,int *x,int *y) {
	if(w->cn<=i) return 0;
	*x=w->corners[i].x;
	*y=w->corners[i].y;
	return 1;
}

struct wire *pick_wire_corner(int x,int y,int *no,int nth) {
	int i;
	for(i=0;i<wiren;i++) {
		struct corner *c=wires[i].corners;
		int j;
		for(j=0;j<wires[i].cn;j++) {
			int dx=c[j].x-x;
			int dy=c[j].y-y;
			int d=dx*dx+dy*dy;
			if(d<2500) {
				if(--nth<=0) {
					*no=j;
					return &wires[i];
				}
			}
		}
	}
	return 0;
}


struct wire *wire(unsigned int i) {
	if(i<wiren) return wires+i;
	return 0;
}

struct element *wire_a(struct wire *w,int *p) { *p=w->ap; return w->a; }
struct element *wire_b(struct wire *w,int *p) { *p=w->bp; return w->b; }

void wires_load(FILE *f) {
	int i;
	for(i=0;i<wiren;i++) { free(wires[i].corners); }
	wiren=0;

	struct wire *cw;
        for(;;) {
                int c=fgetc(f);
                if(c=='W') {
			char na[32],nb[32];
			int pa,pb;
                        fscanf(f," %31s %d %31s %d\n",na,&pa,nb,&pb);
			struct element *a=element_find(na);
			struct element *b=element_find(nb);
			cw=make_wire(a,pa,b,pb);
		} else if(c=='C') {
			int x,y;
                        fscanf(f," %d %d\n",&x,&y);
			wire_add_corner(cw,cw->cn,x,y);
		} else break;
	}


}



