#include <cairo.h>

#include "elements.h"

void draw(cairo_t *c) {
	int i;
	for(i=0;;i++) {
		struct element *e=element(i);
		if(!e) break;

		cairo_rectangle(c,element_x(e),element_y(e),10,10);
		cairo_fill(c);
	}
}

