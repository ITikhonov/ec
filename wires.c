#include <stdio.h>
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

struct wire *find_wire(struct element *a,int ap,struct element *b,int bp) {
	int i;
	for(i=0;i<wiren;i++) {
		struct wire *w=&wires[i];
		if(w->a==a && w->b==b && w->ap==ap && w->bp==bp) return w;
	}
	return 0;
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
		pin_center(w->a,w->ap,x,y);
	} else if(i==w->cn+1) {
		pin_center(w->b,w->bp,x,y);
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

struct element *wire_a(struct wire *w,int *p) { if(p) *p=w->ap; return w->a; }
struct element *wire_b(struct wire *w,int *p) { if(p) *p=w->bp; return w->b; }


// a 1 e 3 4 5 b

void wire_insert(struct wire *w,int n,struct element *e) {
	struct wire *w1=make_wire(e,2,w->b,w->bp);
	w->b=e; w->bp=1;

	int an=n-1;
	int bn=w->cn-n;

	struct corner *ac=malloc(an*sizeof(struct corner));
	struct corner *bc=malloc(bn*sizeof(struct corner));
	int i;
	for(i=0;i<an;i++) { ac[i]=w->corners[i]; }
	for(i=0;i<bn;i++) { ac[i]=w->corners[n+i]; }

	w->corners=realloc(w->corners,0);
	w->corners=ac; w->cn=an;
	w1->corners=bc; w1->cn=bn;
}

void wires_load(FILE *f) {
	struct wire *cw;
        for(;;) {
                int c=fgetc(f);
                if(c=='W') {
			char na[32],nb[32];
			int pa,pb;
                        fscanf(f," %31s %d %31s %d\n",na,&pa,nb,&pb);
			struct element *a=element_find(na);
			struct element *b=element_find(nb);
			cw=find_wire(a,pa,b,pb);
		} else if(c=='C') {
			int x,y;
                        fscanf(f," %d %d\n",&x,&y);
			if(cw) wire_add_corner(cw,cw->cn,x,y);
		} else break;
	}


}

void wires_save(FILE *f) {
	int i;
	for(i=0;i<wiren;i++) {
		struct wire *w=wires+i;
		fprintf(f,"W %s %d %s %d\n",element_name(w->a),w->ap,element_name(w->b),w->bp);

		int j;
		for(j=0;j<w->cn;j++) {
			fprintf(f,"C %d %d\n",w->corners[j].x,w->corners[j].y);
		}
	}
	fprintf(f,".\n");
}

