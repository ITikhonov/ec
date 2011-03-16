#include <stdio.h>
#include <string.h>

#include "elements.h"
#include "packages.h"

struct element {
	char name[32];
	int x,y,a,f;
	struct package *p;
} elements[1024];

int elements_n=0;

struct element *element(unsigned int i) {
	if(i<elements_n) return elements+i;
	return 0;
}

int element_x(struct element *e) { return e->x; }
int element_y(struct element *e) { return e->y; }
char *element_name(struct element *e) { return e->name; }


struct element *element_find(char *s) {
        int i;
	struct element *e;
        for(i=0;(e=element(i));i++) {
		if(strcmp(s,e->name)==0) return e;
	}
	return 0;
}

struct element *element_create(char *s) {
	struct element *e=elements+elements_n++;
	strncpy(e->name,s,31);
	e->p=package("EMPTY");
	return e;
}

void element_setx(struct element *e,int x) { e->x=x; }
void element_sety(struct element *e,int y) { e->y=y; }
void element_seta(struct element *e,int a) { e->a=a; }
void element_setflip(struct element *e,int f) { e->f=f; }

void element_set_package(struct element *e,char *s) {
	struct package *p=package(s);
	if(p) {
		e->p=p;
		printf("set package to %s\n",s);
	}
}



int pin_rect(struct element *e,int pin,int *x,int *y,int *w,int *h) {
	if(!package_pin_rect(e->p,pin,x,y,w,h)) return 0;
	*x+=e->x;
	*y+=e->y;
	return 1;
}


