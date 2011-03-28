#include <cairo.h>
#include <cairo-xlib.h>

#include "packages.h"
#include "elements.h"
#include "commands.h"
#include "wires.h"

static int draw_pin(cairo_t *c,struct element *e,int n) {
	int x[4],y[4];
	if(!pin_rect(e,n,x,y)) return 0;

	cairo_move_to(c,x[0]/10.0,y[0]/10.0);
	cairo_line_to(c,x[1]/10.0,y[1]/10.0);
	cairo_line_to(c,x[2]/10.0,y[2]/10.0);
	cairo_line_to(c,x[3]/10.0,y[3]/10.0);
	cairo_close_path(c);
	if(!element_f(e)) { cairo_fill_preserve(c); }
	cairo_stroke(c);

	return 1;
}

void draw(cairo_t *c) {
	int i;

	{ cairo_save(c);
		cairo_set_source_rgb(c,0.8,0.8,0.8);
		cairo_set_line_width(c,1);

		int w=cairo_image_surface_get_width (cairo_get_target(c));
		int h=cairo_image_surface_get_width (cairo_get_target(c));
		for(i=0;i<w;i+=10) { cairo_move_to(c,i,0); cairo_line_to(c,i,h); }
		for(i=0;i<h;i+=10) { cairo_move_to(c,0,i); cairo_line_to(c,w,i); }
		cairo_stroke(c);
	cairo_restore(c); }

	int spin;
	struct element *se=selected(&spin);

	cairo_save(c);
	for(i=0;;i++) {
		struct element *e=element(i);
		if(!e) break;

		if(e==se && spin==-1) { cairo_set_source_rgb(c,255,0,0); }
		else { cairo_set_source_rgb(c,0,0,0); }

		cairo_move_to(c,element_x(e)/10.0,element_y(e)/10.0-3);
		cairo_show_text(c,element_name(e));

		int j,x,y;
		for(j=1;draw_pin(c,e,j);j++) { ; }

		for(j=0;;j++) {
			int r;
			switch((r=body_line(e,j,&x,&y))) {
			case PL_MOVE: cairo_move_to(c,x/10.0,y/10.0); break;
			case PL_LINE: cairo_line_to(c,x/10.0,y/10.0); break;
			case PL_CLOSE: cairo_close_path(c); break;
			default: goto end;
			}
			printf("line %u %d %d\n",r,x,y);
		}
		end: cairo_stroke(c);

	}
	cairo_restore(c);

	cairo_save(c);
	cairo_set_line_join(c,CAIRO_LINE_JOIN_ROUND);

	int sc;
	struct wire *sw=selected_wire_corner(&sc);
	struct wire *w;
	for(i=0;(w=wire(i));i++) {
		int j,x,y;
		wire_corner(w,0,&x,&y);
		cairo_move_to(c,x/10.0,y/10.0);

		for(j=1;wire_corner(w,j,&x,&y);j++) {
			cairo_line_to(c,x/10.0,y/10.0);
		}
		cairo_stroke(c);
	}

	cairo_restore(c);

	if(sw) {
		int x,y;
		if(wire_corner(sw,sc,&x,&y)) {
			cairo_rectangle(c,x/10.0-5,y/10.0-5,10,10);
			cairo_stroke(c);

			int x1,y1;
			if(!wire_corner(sw,sc+1,&x1,&y1)) {
				wire_corner(sw,sc-1,&x1,&y1);
			}
			cairo_save(c);
			cairo_set_source_rgb(c,0,255,128);
			cairo_move_to(c,x/10.0,y/10.0);
			cairo_line_to(c,x1/10.0,y1/10.0);
			cairo_stroke(c);
			cairo_restore(c);
		}
	}
}

