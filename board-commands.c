#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board-commands.h"
#include "board-elements.h"
#include "board-wires.h"

static struct board_element *e;

static struct board_wire *w;
static int wc=-1;
static int wnth=0;

struct board_element *selected_element() { return e; }
struct board_wire *selected_wire_corner(int *n) { *n=wc; return w; }

static void select_element(char *s) { e=board_element_find(s); }

enum {HAS_X=1,HAS_Y=2,HAS_A=4,HAS_F=8};

static int convert_at(char *s,int*x,int*y,int*a,int*f) {
	char *p;
	int r=0;
	*x=strtol(s,&p,10); if(s!=p) r|=HAS_X; if(*p==0) return r; s=p+1;
	*y=strtol(s,&p,10); if(s!=p) r|=HAS_Y; if(*p==0) return r; s=p+1;
	*a=strtol(s,&p,10); if(s!=p) r|=HAS_A; if(*p==0) return r; s=p+1;
	*f=strtol(s,&p,10); if(s!=p) r|=HAS_F; return r;
}


static void pos(char *s) {
	int r,x,y,a,f;
	r=convert_at(s,&x,&y,&a,&f);

	if(e) {
		if(r&HAS_X) { board_element_setx(e,x); }
		if(r&HAS_Y) { board_element_sety(e,y); }
		if(r&HAS_A) { board_element_seta(e,a); }
		if(r&HAS_F) { board_element_setf(e,f); }
	} else {
		w=board_pick_corner(x,y,&wc,wnth++);
		if(w==0) {
			wnth=0;
			w=board_pick_corner(x,y,&wc,wnth++);
		}
	}
}

static void save(char *s) {
	FILE *f=fopen(s,"w");
	board_elements_save(f);
	board_wires_save(f);
	fclose(f);
}

void load_board(char *s) {
	FILE *f=fopen(s,"r");
	board_elements_load(f);
	board_wires_load(f);
	fclose(f);
}

static void move(char *s) {
	int x,y;
	sscanf(s,"%d.%d",&x,&y);
	if(w && wc>=0) {
		board_corner_move(w,wc,x,y);
	}
}

static void add() {
	if(w) {
		int x,y,x1,y1;
		if(board_corner(w,wc,&x,&y)) {
			if(!board_corner(w,wc+1,&x1,&y1)) {
				board_corner(w,wc-1,&x1,&y1);
			}
			x=(x+x1)/2;
			y=(y+y1)/2;

			wc=board_add_corner(w,wc,x,y);
		}
	}
}

static void hide() {
	board_element_seth(e,!board_element_h(e));
}

int command(char *s) {
	printf("command '%s'\n",s);
	switch(*s) {
	case '\0': add(); break;
	case 'A'...'Z': select_element(s); break;
	case '@': pos(s+1); break;
	case '!': move(s+1); break;
	case 'h': hide(); break;
	case 's': save(s+1); break;
	case 'l': load_board(s+1); break;
	case 'q': return 1;
	}
	return 0;
}

