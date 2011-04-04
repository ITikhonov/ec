#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "elements.h"
#include "board-elements.h"
#include "packages.h"

static struct element {
	char name[32];
	struct package *p;
} elements[1024];

static int elements_n=0;

char *element(unsigned int i) {
	if(i<elements_n) return elements[i].name;
	return 0;
}

static struct element *element_find(char *s) {
        int i;
        for(i=0;i<elements_n;i++) {
		struct element *e=&elements[i];
		if(strcmp(s,e->name)==0) return e;
	}
	return 0;
}

char *element_create(char *s) {
	struct element *e;
	if((e=element_find(s))) return e->name;

	e=elements+elements_n;
	strncpy(e->name,s,31);
	e->p=0;
	elements_n++;
	return e->name;
}

struct board_element *board_element(char *s) {
	struct board_element *e=board_element_find(s);
	if(e) return e;
	return board_element_create(s);
}

struct package *element_package(char *e) { return element_find(e)->p; }

void element_set_package(char *name,char *s) {
	struct element *e=element_find(name);
	if(!e) return;
	free(e->p);
	struct package *p=package(s);
	e->p=p;
}

