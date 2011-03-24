#include <cairo.h>

#include "elements.h"
#include "commands.h"
#include "wires.h"

void draw(cairo_t *c) {
	int i;
	int spin;
	struct element *se=selected(&spin);

	for(i=0;;i++) {
		struct element *e=element(i);
		if(!e) break;

		if(e==se && spin==-1) { cairo_set_source_rgb(c,255,0,0); }
		else { cairo_set_source_rgb(c,0,0,0); }

		cairo_move_to(c,element_x(e)/10.0,element_y(e)/10.0-3);
		cairo_show_text(c,element_name(e));
		int j,x,y,w,h;
		for(j=1;pin_rect(e,j,&x,&y,&w,&h);j++) {
			if(e==se && spin!=-1) {
				 if(spin==j) { cairo_set_source_rgb(c,255,0,0); }
				 else { cairo_set_source_rgb(c,0,0,0); }
			}
			cairo_rectangle(c,x/10.0,y/10.0,w/10.0,h/10.0);
			cairo_fill(c);

		}
	}

	struct wire *w;
	for(i=0;(w=wire(i));i++) {
		int x0,y0,w0,h0,x1,y1,p;

		struct element *e=wire_a(w,&p);
		if(!pin_rect(e,p,&x0,&y0,&w0,&h0)) continue;
		x0+=w0/2; y0+=h0/2;
		cairo_move_to(c,x0/10.0,y0/10.0);

		int j,x,y;
		for(j=0;wire_corner(w,j,&x,&y);j++) {
			cairo_line_to(c,x/10.0,y/10.0);
		}


		e=wire_b(w,&p);
		if(!pin_rect(e,p,&x1,&y1,&w0,&h0)) continue;
		x1+=w0/2; y1+=h0/2;
		cairo_line_to(c,x1/10.0,y1/10.0);
		cairo_stroke(c);

	}
}

