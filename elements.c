#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "elements.h"
#include "packages.h"

struct element {
	char name[32];
	int x,y,a,f;
	int h;
	struct package *p;
} elements[1024];

int elements_n=0;

struct element *element(unsigned int i) {
	if(i<elements_n) return elements+i;
	return 0;
}

int element_x(struct element *e) { return e->x; }
int element_y(struct element *e) { return e->y; }
int element_f(struct element *e) { return e->f; }
int element_h(struct element *e) { return e->h; }
char *element_name(struct element *e) { return e->name; }


struct element *element_find(char *s) {
        int i;
	struct element *e;
        for(i=0;(e=element(i));i++) {
		if(strcmp(s,e->name)==0) return e;
	}
	printf("EF: %s not found\n",s);
	return 0;
}

static void autoname(char *s,int l,char *name) {
	int i=1;
	for(;i<200;i++) {
		snprintf(name,32,"%.*s%u",l,s,i);
		printf("autoname %s\n",name);
		if(!element_find(name)) break;
	}
}

struct element *element_create(char *s) {
	struct element *e=elements+elements_n;
	int l=strlen(s)-1;
	if(s[l]=='?') { autoname(s,l,e->name); }
	else { strncpy(e->name,s,31); }
	e->p=package("EMPTY");
	e->x=e->y=e->a=e->f=e->h=0;
	elements_n++;
	return e;
}

void element_setx(struct element *e,int x) { e->x=x; }
void element_sety(struct element *e,int y) { e->y=y; }
void element_seta(struct element *e,int a) { e->a=a; }
void element_setflip(struct element *e,int f) { e->f=f; }

void element_seth(struct element *e,int h) { e->h=h; }

void element_set_package(struct element *e,char *s) {
	struct package *p=package(s);
	if(p) {
		e->p=p;
		printf("set package to %s\n",s);
	}
}

static void rotate(int a,int *x,int *y) {
	int x0=*x; int y0=*y;
	float r=(M_PI/180.0)*a;
	float c=cos(r);
	float s=sin(r);

	*x=x0*c-y0*s;
	*y=x0*s+y0*c;
}

int pin_rect(struct element *e,int pin,int x[4],int y[4]) {
	int x0,y0,w,h;
	if(!package_pin_rect(e->p,pin,&x0,&y0,&w,&h)) return 0;


	x[0]=x0;	y[0]=y0;
	x[1]=x0+w;	y[1]=y0;
	x[2]=x0+w;	y[2]=y0+h;
	x[3]=x0;	y[3]=y0+h;

	if(element_f(e)) { x[0]*=-1; x[1]*=-1; x[2]*=-1; x[3]*=-1; }


	if(e->a) {
		rotate(e->a,&x[0],&y[0]);
		rotate(e->a,&x[1],&y[1]);
		rotate(e->a,&x[2],&y[2]);
		rotate(e->a,&x[3],&y[3]);
	}
	

	int ex=element_x(e);
	int ey=element_y(e);
	x[0]+=ex; x[1]+=ex; x[2]+=ex; x[3]+=ex;
	y[0]+=ey; y[1]+=ey; y[2]+=ey; y[3]+=ey;



	return 1;
}


int pin_center(struct element *e,int pin,int *x,int *y) {
	int w,h;
	if(!package_pin_rect(e->p,pin,x,y,&w,&h)) return 0;
	*x+=w/2; *y+=h/2;
	if(e->f) { *x=-*x; }
	if(e->a) { rotate(e->a,x,y); }
	*x+=e->x;
	*y+=e->y;
	return 1;
}

int body_line(struct element *e,unsigned int n,int *x,int *y) {
	int r=package_line(e->p,n,x,y);
	if(e->f) { *x=-*x; }
	if(e->a) { rotate(e->a,x,y); }
	*x+=e->x;
	*y+=e->y;
	return r;
}


void elements_save(FILE *f) {
        int i;
	struct element *e;
        for(i=0;(e=element(i));i++) {
		char p[32];
		package_name(e->p,p);
		fprintf(f,"E %s %d %d %d %d %d %s\n",e->name,e->x,e->y,e->a,e->f,e->h,p);
	}
	fprintf(f,".\n");
}

void elements_load(FILE *f) {
	elements_n=0;
	for(;;) {
		int c=fgetc(f);
		printf("EL: %c\n",c);
		if(c=='E') {
			char name[32];
			int x,y,a,fl,h;
			char pn[32];
			fscanf(f," %31s %d %d %d %d %d %31s\n",name,&x,&y,&a,&fl,&h,pn);
			struct element *e=element_create(name);
			e->x=x; e->y=y; e->a=a; e->f=fl; e->h=h;
			if(e->p) free(e->p);
			e->p=package(pn);
		} else { fgetc(f); break; }
	}
}

