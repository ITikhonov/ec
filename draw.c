#include <cairo.h>
#include <cairo-xlib.h>

#include "packages.h"
#include "elements.h"
#include "wires.h"
#include "board-elements.h"
#include "board-wires.h"
#include "board-commands.h"
#include "colors.h"

static int draw_pin(cairo_t *c,struct board_element *e,struct package *p,int n) {
	int x[4],y[4];

	if(!package_pin_rect(p,n,x,y)) return 0;
	board_pin_rect(e,x,y);

	cairo_save(c);
	cairo_move_to(c,x[0]/10.0,y[0]/10.0);
	cairo_line_to(c,x[1]/10.0,y[1]/10.0);
	cairo_line_to(c,x[2]/10.0,y[2]/10.0);
	cairo_line_to(c,x[3]/10.0,y[3]/10.0);
	cairo_close_path(c);
	if(!board_element_f(e)) { cairo_fill_preserve(c); }
	cairo_stroke(c);

	cairo_restore(c);
	return 1;
}

void draw(cairo_t *c) {
	int i;
	struct board_element *se=selected_element();

	cairo_save(c);
	char *e0;
	for(i=0;(e0=element(i));i++) {
		struct board_element *e=board_element(e0);

		if(e==se) { cairo_set_source_rgb(c,1,0,0); }
		else if(board_element_h(e)) { cairo_set_source_rgb(c,0.7,0.7,0.7); }
		else { cairo_set_source_rgb(c,0,0,0); }

		cairo_move_to(c,board_element_x(e)/10.0,board_element_y(e)/10.0-3);
		cairo_show_text(c,e0);

		int j,x,y;
		struct package *p=element_package(e0);
		for(j=1;draw_pin(c,e,p,j);j++) { ; }

		cairo_save(c);
		for(j=0;;j++) {
			int r=package_line(p,j,&x,&y);
			board_body_line(e,&x,&y);
			switch(r) {
			case PL_MOVE: cairo_move_to(c,x/10.0,y/10.0); break;
			case PL_LINE: cairo_line_to(c,x/10.0,y/10.0); break;
			case PL_CLOSE: cairo_close_path(c); break;
			default: goto end;
			}
		}
		end: cairo_stroke(c);
		cairo_restore(c);
	}
	cairo_restore(c);

	cairo_save(c);
	cairo_set_line_join(c,CAIRO_LINE_JOIN_ROUND);

	int sc;
	struct board_wire *sw=selected_wire_corner(&sc);
	struct wire *w0;
	for(i=0;(w0=wire(i));i++) {
		int j,x,y;
		if(board_element_h(board_element(wire_a(w0)))) continue;
		if(board_element_h(board_element(wire_b(w0)))) continue;

		struct board_wire *w=board_wire(w0);

		board_corner(w,0,&x,&y);
		int cn=i%ncolors;
		cairo_set_source_rgb(c,colors[cn][0],colors[cn][1],colors[cn][2]);
		cairo_set_line_width(c,5);
		cairo_move_to(c,x/10.0,y/10.0);

		for(j=1;board_corner(w,j,&x,&y);j++) {
			cairo_line_to(c,x/10.0,y/10.0);
		}
		cairo_stroke(c);
	}

	cairo_restore(c);

	if(sw) {
		int x,y;
		if(board_corner(sw,sc,&x,&y)) {
			cairo_rectangle(c,x/10.0-5,y/10.0-5,10,10);
			cairo_stroke(c);

			int x1,y1;
			if(!board_corner(sw,sc+1,&x1,&y1)) {
				board_corner(sw,sc-1,&x1,&y1);
			}
			cairo_save(c);
			cairo_set_source_rgb(c,0,1,0.5);
			cairo_move_to(c,x/10.0,y/10.0);
			cairo_line_to(c,x1/10.0,y1/10.0);
			cairo_stroke(c);
			cairo_restore(c);
		}
	}
}

