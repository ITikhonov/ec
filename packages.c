#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct package {
	int (*pin_rect)(struct package *,int,int *x, int *y, int *w, int *h);
};

static int empty_pin_rect(struct package *p0,int n,int *x,int *y,int *w,int *h) {
	return 0;
}

static struct package *empty(char *args) {
	struct package *p=malloc(sizeof(struct package));
	p->pin_rect=empty_pin_rect;
	return p;
}

/******************************************************
 * TQFP
 ******************************************************
 */

struct tqfp {
	struct package p;
	int pins;
};

static int tqfp_pin_rect(struct package *p0,int n,int *x,int *y,int *w,int *h) {
	struct tqfp *p=(struct tqfp*)p0;

	if(n>p->pins) return 0;

	n-=1;
	int side=n/(p->pins/4);
	switch(side) {
	case 0: // Left
		*x=0;
		*y=n*80;
		*w=100;
		*h=37;
		return 1;
	case 1: // Bottom
		*x=n*80;
		*y=1100;
		*w=37;
		*h=100;
		return 1;
	case 2: // Right
		*x=1100;
		*y=1100-n*80;
		*w=100;
		*h=37;
		return 1;
	case 3: // Top
		*x=1100-n*80;
		*y=0;
		*w=37;
		*h=100;
		return 1;
	}
	return 0;
}

static struct package *tqfp(char *args) {
	struct tqfp *p=malloc(sizeof(struct tqfp));
	p->p.pin_rect=tqfp_pin_rect;
	p->pins=atoi(args);
	return (struct package*)p;
}

struct package *package(char *name) {
	char *p=strchrnul(name,'.');
	int len=p-name;

	if(len==5 && strncmp(name,"EMPTY",5)==0) {
		return empty(p+1);
	} else if(len==4 && strncmp(name,"TQFP",4)==0) {
		return tqfp(p+1);
	}
	return 0;
}

int package_pin_rect(struct package *p,int n,int *x, int *y, int *w, int *h) {
	return p->pin_rect(p,n,x,y,w,h);
}

