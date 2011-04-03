#include <stdio.h>
#include <string.h>

#include "packages.h"
#include "packages/package.h"

struct package *make_package(char *name, int l, char *arg);

struct package *package(char *name) {
	char *p=strchr(name,'.');
	int len=p?(p-name):strlen(name);
	return make_package(name,len,p?(p+1):(name+len));
}

void package_name(struct package *p, char name[32]) {
	p->name(p,name);
}

int package_pin_rect(struct package *p,int n,int x[4], int y[4]) {
	int w,h;
	if(!p->pin_rect(p,n,x,y,&w,&h)) return 0;
	x[1]=x[0]+w; y[1]=y[0];
	x[2]=x[0]+w; y[2]=y[0]+h;
	x[3]=x[0]; y[3]=y[0]+h;
	return 1;
}

int package_line(struct package *p,unsigned int n,int *x0,int *y0) {
	return p->line(p,n,x0,y0);
}

