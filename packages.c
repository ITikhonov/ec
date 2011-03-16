#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct package *tqfp(char *args) {
	int pins=atoi(args);
	printf("pins %u\n",pins);
}

struct package *package(char *name) {
	char *p=strchrnul(name,'.');
	int len=p-name;

	if(len==4 && strncmp(name,"TQFP",4)==0) {
		return tqfp(p+1);
	}
}


void test() __attribute__((constructor));

void test() {
    package("TQFP.44");
}

