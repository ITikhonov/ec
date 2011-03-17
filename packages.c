#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "packages.h"

struct package {
	int (*pin_rect)(struct package *,int,int *x, int *y, int *w, int *h);
	void (*name)(struct package *,char name[32]);
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
 * Rectangular example: R.0805
 ******************************************************
 */

struct rect {
	struct package p;
	int size;
};

static void rect_name(struct package *p0, char name[32]) {
	struct rect *p=(struct rect*)p0;
	snprintf(name,32,"R.%u",p->size);
}

static int rect_pin_rect(struct package *p0,int n,int *x,int *y,int *w,int *h) {
	if(n>2 || n<1) return 0;
	struct rect *p=(struct rect*)p0;

	switch(p->size) {
	case 805:
		if(n==1) { *x=0; *y=0; *w=40; *h=125; }
		else if(n==2) { *x=160; *y=0; *w=40; *h=125; }
		break;
	default:
		return 0;
	}

	return 1;
}

static struct package *rect(char *args) {
	struct rect *p=malloc(sizeof(struct rect));
	p->p.pin_rect=rect_pin_rect;
	p->p.name=rect_name;
	p->size=atoi(args);
	return (struct package*)p;
}

/******************************************************
 * TQFP
 ******************************************************
 */

struct tqfp {
	struct package p;
	int pins;
};

static void tqfp_name(struct package *p0, char name[32]) {
	struct tqfp *p=(struct tqfp*)p0;
	snprintf(name,32,"TQFP.%u",p->pins);
}

static int tqfp_pin_rect(struct package *p0,int n,int *x,int *y,int *w,int *h) {
	struct tqfp *p=(struct tqfp*)p0;

	if(n>p->pins) return 0;

	n-=1;
	int side=n/(p->pins/4);
	switch(side) {
	case 0: // Left
		*x=0;
		*y=n*80+181;
		*w=100;
		*h=37;
		return 1;
	case 1: // Bottom
		*x=(n-11)*80+181;
		*y=1100;
		*w=37;
		*h=100;
		return 1;
	case 2: // Right
		*x=1100;
		*y=1200-181-(n-22)*80;
		*w=100;
		*h=-37;
		return 1;
	case 3: // Top
		*x=1200-181-(n-33)*80;
		*y=0;
		*w=-37;
		*h=100;
		return 1;
	}
	return 0;
}

static struct package *tqfp(char *args) {
	struct tqfp *p=malloc(sizeof(struct tqfp));
	p->p.pin_rect=tqfp_pin_rect;
	p->p.name=tqfp_name;
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
	} else if(len==1 && strncmp(name,"R",1)==0) {
		return rect(p+1);
	}
	return 0;
}

void package_name(struct package *p, char name[32]) {
	p->name(p,name);
}

int package_pin_rect(struct package *p,int n,int *x, int *y, int *w, int *h) {
	return p->pin_rect(p,n,x,y,w,h);
}

