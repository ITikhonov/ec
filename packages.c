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

int package_pin_rect(struct package *p,int n,int *x, int *y, int *w, int *h) {
	return p->pin_rect(p,n,x,y,w,h);
}

int package_line(struct package *p,unsigned int n,int *x0,int *y0) {
	return p->line(p,n,x0,y0);
}

