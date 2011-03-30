#include <stdio.h>
#include <stdlib.h>

#include "../packages.h"

#include "package.h"

#define INIT struct package *PACKAGE(char *args) { \
        struct pk *p=malloc(sizeof(struct pk)); \
        p->p.pin_rect=p_pin_rect; \
        p->p.name=p_name; \
        p->p.line=p_line;

#define END return (struct package*)p; }


#define NAME static void p_name(struct package *p0, char name[32]) { struct pk *p=(struct pk*)p0; (void)p;
#define PIN static int p_pin_rect(struct package *p0,int n,int *x,int *y,int *w,int *h) { struct pk *p=(struct pk*)p0; (void)p;
#define LINE static int p_line(struct package *p0,unsigned int n,int *x,int *y) { struct pk *p=(struct pk*)p0; (void)p;

