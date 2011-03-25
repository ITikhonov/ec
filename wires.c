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

void wire_corner_move(struct wire *w,int n,int x,int y) {
	if(n==0) return;
	if(n>w->cn) return;
	w->corners[n-1].x=x;
	w->corners[n-1].y=y;
}


struct wire *make_wire(struct element *a,int ap,struct element *b,int bp) {
	struct wire *w=wires+wiren++;
	w->a=a; w->ap=ap;
	w->b=b; w->bp=bp;
	w->corners=0;
	w->cn=0;
	return w;
}

int wire_add_corner(struct wire *w,int after,int x,int y) {
	int cn=w->cn++;
	w->corners=realloc(w->corners,w->cn*sizeof(struct corner));

	if(after==w->cn) after--;

	int i;
	for(i=cn;i>after;i--) {
		w->corners[i]=w->corners[i-1];
	}
	w->corners[after].x=x;
	w->corners[after].y=y;
	return after+1;
}

int wire_corner(struct wire *w,unsigned int i,int *x,int *y) {
	if(i==0) {
		int w0,h0;
		pin_rect(w->a,w->ap,x,y,&w0,&h0);
                *x+=w0/2; *y+=h0/2;
	} else if(i==w->cn+1) {
		int w0,h0;
		pin_rect(w->b,w->bp,x,y,&w0,&h0);
                *x+=w0/2; *y+=h0/2;
	} else if(i<=w->cn) {
		*x=w->corners[i-1].x;
		*y=w->corners[i-1].y;
	} else {
		return 0;
	}
	return 1;
}

struct wire *pick_wire_corner(int x,int y,int *no,int nth) {
	int i;
	for(i=0;i<wiren;i++) {
		int j,cx,cy;
		for(j=0;wire_corner(&wires[i],j,&cx,&cy);j++) {
			int dx=cx-x;
			int dy=cy-y;
			int d=dx*dx+dy*dy;
			if(d<5000) {
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



