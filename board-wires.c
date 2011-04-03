#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board-wires.h"

struct corner { int x,y; };

struct board_wire {
	char a[32];
	int ap;
	char b[32];
	int bp;

	struct corner *corners;
	int cn;
} wires[1024];
static int wiren=0;

void board_corner_move(struct board_wire *w,int n,int x,int y) {
	if(n==0) return;
	if(n>w->cn) return;
	w->corners[n-1].x=x;
	w->corners[n-1].y=y;
}

struct board_wire *board_find_wire(char *a,int ap,char *b,int bp) {
	int i;
	for(i=0;i<wiren;i++) {
		struct board_wire *w=&wires[i];
		if(strcmp(w->a,a)==0 && strcmp(w->b,b)==0 && w->ap==ap && w->bp==bp) return w;
	}
	return 0;
}

struct board_wire *board_make_wire(char *a,int ap,char *b,int bp) {
	struct board_wire *w=wires+wiren++;
	strcpy(w->a,a); strcpy(w->b,b);
	w->ap=ap; w->bp=bp;
	w->corners=0;
	w->cn=0;
	return w;
}

int board_add_corner(struct board_wire *w,int after,int x,int y) {
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

int board_corner(struct board_wire *w,unsigned int i,int *x,int *y) {
	if(i<=w->cn) {
		*x=w->corners[i-1].x;
		*y=w->corners[i-1].y;
	} else {
		return 0;
	}
	return 1;
}

struct board_wire *board_pick_corner(int x,int y,int *no,int nth) {
	int i;
	for(i=0;i<wiren;i++) {
		int j,cx,cy;
		for(j=0;board_corner(&wires[i],j,&cx,&cy);j++) {
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


struct board_wire *board_wire(unsigned int i) {
	if(i<wiren) return wires+i;
	return 0;
}

char *board_wire_a(struct board_wire *w) { return w->a; }
char *board_wire_b(struct board_wire *w) { return w->b; }
int board_wire_ap(struct board_wire *w) { return w->ap; }
int board_wire_bp(struct board_wire *w) { return w->bp; }


// a 1 e 3 4 5 b

void board_wire_insert(struct board_wire *w,int n,char *e) {
	struct board_wire *w1=board_make_wire(e,2,w->b,w->bp);
	strcpy(w->b,e); w->bp=1;

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

void board_wires_load(FILE *f) {
	struct board_wire *cw;
        for(;;) {
                int c=fgetc(f);
                if(c=='W') {
			char a[32],b[32];
			int ap,bp;
                        fscanf(f," %31s %d %31s %d\n",a,&ap,b,&bp);
			cw=board_find_wire(a,ap,b,bp);
		} else if(c=='C') {
			int x,y;
                        fscanf(f," %d %d\n",&x,&y);
			if(cw) board_add_corner(cw,cw->cn,x,y);
		} else break;
	}
}

void board_wires_save(FILE *f) {
	int i;
	for(i=0;i<wiren;i++) {
		struct board_wire *w=wires+i;
		fprintf(f,"W %s %d %s %d\n",w->a,w->ap,w->b,w->bp);

		int j;
		for(j=0;j<w->cn;j++) {
			fprintf(f,"C %d %d\n",w->corners[j].x,w->corners[j].y);
		}
	}
	fprintf(f,".\n");
}
