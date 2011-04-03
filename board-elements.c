#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "board-elements.h"
#include "packages.h"

static struct board_element {
	char name[32];
	int x,y,a,f,h;
} elements[1024];

static int elements_n=0;

struct board_element *board_element(unsigned int i) {
	if(i<elements_n) return elements+i;
	return 0;
}

int board_element_x(struct board_element *e) { return e->x; }
int board_element_y(struct board_element *e) { return e->y; }
int board_element_f(struct board_element *e) { return e->f; }
int board_element_h(struct board_element *e) { return e->h; }
char *board_element_name(struct board_element *e) { return e->name; }


struct board_element *board_element_find(char *s) {
        int i;
	struct board_element *e;
        for(i=0;(e=board_element(i));i++) {
		if(strcmp(s,e->name)==0) return e;
	}
	return 0;
}

struct board_element *board_element_create(char *s) {
	struct board_element *e=elements+elements_n;
	strncpy(e->name,s,31);
	e->x=e->y=e->a=e->f=e->h=0;
	elements_n++;
	return e;
}

void board_element_setx(struct board_element *e,int x) { e->x=x; }
void board_element_sety(struct board_element *e,int y) { e->y=y; }
void board_element_seta(struct board_element *e,int a) { e->a=a; }
void board_element_setf(struct board_element *e,int f) { e->f=f; }
void board_element_seth(struct board_element *e,int h) { e->h=h; }

static void rotate(int a,int *x,int *y) {
	int x0=*x; int y0=*y;
	float r=(M_PI/180.0)*a;
	float c=cos(r);
	float s=sin(r);

	*x=x0*c-y0*s;
	*y=x0*s+y0*c;
}

void board_pin_rect(struct board_element *e,int x[4],int y[4]) {
	if(board_element_f(e)) { x[0]*=-1; x[1]*=-1; x[2]*=-1; x[3]*=-1; }
	if(e->a) {
		rotate(e->a,&x[0],&y[0]);
		rotate(e->a,&x[1],&y[1]);
		rotate(e->a,&x[2],&y[2]);
		rotate(e->a,&x[3],&y[3]);
	}

	int ex=board_element_x(e);
	int ey=board_element_y(e);
	x[0]+=ex; x[1]+=ex; x[2]+=ex; x[3]+=ex;
	y[0]+=ey; y[1]+=ey; y[2]+=ey; y[3]+=ey;
}


void board_pin_center(struct board_element *e,int x[4],int y[4]) {
	board_pin_rect(e,x,y);
	x[0]=(x[0]+x[1]+x[2]+x[3])/4;
	y[0]=(y[0]+y[1]+y[2]+y[3])/4;
}

void board_body_line(struct board_element *e,int *x,int *y) {
	if(e->f) { *x=-*x; }
	if(e->a) { rotate(e->a,x,y); }
	*x+=e->x;
	*y+=e->y;
}


void board_elements_save(FILE *f) {
        int i;
	struct board_element *e;
        for(i=0;(e=board_element(i));i++) {
		fprintf(f,"E %s %d %d %d %d %d\n",e->name,e->x,e->y,e->a,e->f,e->h);
	}
	fprintf(f,".\n");
}

void board_elements_load(FILE *f) {
	elements_n=0;
	for(;;) {
		int c=fgetc(f);
		printf("EL: %c\n",c);
		if(c=='E') {
			char name[32];
			int x,y,a,fl,h;
			fscanf(f," %31s %d %d %d %d %d\n",name,&x,&y,&a,&fl,&h);
			struct board_element *e=board_element_create(name);
			if(e) { e->x=x; e->y=y; e->a=a; e->f=fl; e->h=h; }
		} else { fgetc(f); break; }
	}
}

