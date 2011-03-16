#include <cairo.h>

#include "elements.h"

void draw(cairo_t *c) {
	int i;
	for(i=0;;i++) {
		struct element *e=element(i);
		if(!e) break;

		cairo_move_to(c,element_x(e)/10.0,element_y(e)/10.0-3);
		cairo_show_text(c,element_name(e));
		int j,x,y,w,h;
		for(j=1;pin_rect(e,j,&x,&y,&w,&h);j++) {
			cairo_rectangle(c,x/10.0,y/10.0,w/10.0,h/10.0);
			cairo_fill(c);

		}
	}
}

