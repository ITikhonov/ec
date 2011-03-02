#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <cairo.h>
#include <cairo-xlib.h>

#include <X11/Xutil.h>

#define PI 3.1415926535

typedef struct win {
    Display *dpy;
    int scr;

    Window win;
    GC gc;

    int width, height;
    KeyCode quit_code;
} win_t;

static void win_init(win_t *win);
static void win_deinit(win_t *win);
static void win_draw();
static void win_handle_events(win_t *win);

win_t win;

int addtag(int x,int y,int p);
void addwire(int a, int ap, int b, int bp);

int
main(int argc, char *argv[])
{

    int a=addtag(1000,1000,0);
    int b=addtag(3000,1500,0);

    addwire(a,1,b,9);

    win.dpy = XOpenDisplay(0);

    if (win.dpy == NULL) {
	fprintf(stderr, "Failed to open display\n");
	return 1;
    }

    win_init(&win);

    win_draw();

    win_handle_events(&win);

    win_deinit(&win);

    XCloseDisplay(win.dpy);

    return 0;
}

void draw(cairo_t *cr);

cairo_surface_t *overlay_s=0;
cairo_t *overlay=0;

static void win_draw()
{
    cairo_surface_t *surface;
    cairo_t *cr;
    Visual *visual = DefaultVisual(win.dpy, DefaultScreen (win.dpy));

    XClearWindow(win.dpy, win.win);

    surface=cairo_xlib_surface_create(win.dpy, win.win, visual,
					 win.width, win.height);
    cr = cairo_create(surface);
    cairo_set_source_rgb(cr,0,0,0);
    cairo_set_font_size (cr,100);
    cairo_scale(cr,0.1,0.1);

    draw(cr);
    cairo_identity_matrix(cr);
    cairo_set_source_surface(cr,overlay_s,0,0);
    cairo_paint (cr);

    if (cairo_status (cr)) {
	printf("Cairo is unhappy: %s\n",
	       cairo_status_to_string (cairo_status (cr)));
	exit(0);
    }

    cairo_destroy(cr);
    cairo_surface_destroy (surface);
    XFlush(win.dpy);
}

void recreate_overlay() {
	overlay_s=cairo_image_surface_create(CAIRO_FORMAT_ARGB32,win.width,win.height);
	overlay=cairo_create(overlay_s);
	cairo_set_source_rgb(overlay,0,0,0);
	cairo_set_font_size (overlay,100);
	cairo_scale(overlay,0.1,0.1);
}

static void
win_init(win_t *win)
{
    Window root;

    win->width = 400;
    win->height = 400;

    recreate_overlay();

    root = DefaultRootWindow(win->dpy);
    win->scr = DefaultScreen(win->dpy);

    win->win = XCreateSimpleWindow(win->dpy, root, 0, 0,
				   win->width, win->height, 0,
				   WhitePixel(win->dpy, win->scr), WhitePixel(win->dpy, win->scr));

    win->quit_code = XKeysymToKeycode(win->dpy, XStringToKeysym("Q"));

    XSelectInput(win->dpy, win->win,
		 KeyPressMask
		 |StructureNotifyMask
		 |ExposureMask);

    XMapWindow(win->dpy, win->win);
}

static void
win_deinit(win_t *win)
{
    XDestroyWindow(win->dpy, win->win);
}

/*
 * Packages
 */

void Ptqfp32_pin(int p,int *x,int *y) {
	if(p==0) {
		*x=450;
		*y=450;
		return;
	}

	if(p<9) {
		*x=850;
		*y=140+80*(p-1)+37/2;
		return;
	}

	if(p<17) {
		*x=80*(7-(p-9))+140+37/2;
		*y=850;
		return;
	}

	if(p<25) {
		*x=50;
		*y=80*(7-(p-17))+140+37/2;
		return;
	}
	if(p<33) {
		*x=80*(p-25)+140+37/2;
		*y=50;
		return;
	}
}

void Ptqfp32(cairo_t *c, int x, int y) {
	cairo_save(c);

	cairo_set_source_rgb(c,0,0,0);
	cairo_rectangle(c,x+100,y+100,700,700);
	cairo_stroke(c);

	cairo_arc(c,x+800-30,y+140+37/2,20,0,2*M_PI);
	cairo_fill(c);

	cairo_set_source_rgba(c,0,0,0,0.5);
	int i;

	for(i=0;i<8;i++) { cairo_rectangle(c,x,y+80*i+140,100,37); cairo_fill(c); }
	for(i=0;i<8;i++) { cairo_rectangle(c,x+800,y+80*i+140,100,37); cairo_fill(c); }
	for(i=0;i<8;i++) { cairo_rectangle(c,x+80*i+140,y,37,100); cairo_fill(c); }
	for(i=0;i<8;i++) { cairo_rectangle(c,x+80*i+140,y+800,37,100); cairo_fill(c); }

	cairo_set_source_rgba(c,1,0,0,1);
	for(i=0;i<=32;i++) {
		int rx,ry;
		Ptqfp32_pin(i,&rx,&ry);
		
		cairo_arc(c,x+rx,y+ry,20,0,2*M_PI);
		cairo_fill(c);
	}

	cairo_restore(c);
}

struct package {
	char name[16];
	void (*draw)(cairo_t *,int,int);
	void (*pin)(int,int*,int*);
} package[1024] = {{"TQFP-32",Ptqfp32,Ptqfp32_pin}};

/*
 * Data: tags
 */

struct tag {
	int x,y; // 0.01mm units
	struct package *p;
} tag[1024];
int tagn=0;

int addtag(int x,int y,int p) {
	tag[tagn].x=x;
	tag[tagn].y=y;
	tag[tagn].p=package+p;
	return tagn++;
}



/*
 * Data: wires
 */

struct wire {
	struct { int tag; int pin; } a,b;
} wire[1024];
int wiren=0;

void addwire(int a, int ap, int b, int bp) {
	wire[wiren].a.tag=a;
	wire[wiren].a.pin=ap;
	wire[wiren].b.tag=b;
	wire[wiren].b.pin=bp;
	wiren++;
}

/*
 * o662 - put new tag at 6:6:2
 * 67pt8 - assign tag #67 to be in a package TQFP-32 (7x7mm lead: 1mm length 0.8mm space 0.37mm width)
 * w67/8-o8745-69/2-72 - wire from pin 8 of #67 to 87x45 to pin 2 of 69 to point 72
 * u67/8-o8745-69/2-72 - unwire from pin 8 of #67 to 87x45 to pin 2 of 69 to point 72
 * 67d - delete tag #67
 */

enum { IDLE, TAG, EDIT, WIRE, UNWIRE } state=IDLE;

void grid_step(int step,int *sx,int *sy) {
	double x1,y1,x2,y2;
	cairo_clip_extents(overlay,&x1,&y1,&x2,&y2);
	double sx0=(x2-x1)/3;
	double sy0=(y2-y1)/3;

	if(step) {
		double div=pow(3,step);
		sx0/=div;
		sy0/=div;
		printf("step %u, div %lf\n",step,div);

	}
	*sx=sx0; *sy=sy0;
}

void target_grid(int x, int y, int sx, int sy) {
	int x1,y1,x2,y2;
	x1=x; x2=x+3*sx;
	y1=y; y2=y+3*sy;

	printf("%d %d/%d %d/%d %d\n",x1,x2,y1,y2,sx,sy);

	cairo_save(overlay);

	cairo_move_to(overlay,x1+sx,y1); cairo_line_to(overlay,x1+sx,y2); cairo_stroke(overlay);
	cairo_move_to(overlay,x1+2*sx,y1); cairo_line_to(overlay,x1+2*sx,y2); cairo_stroke(overlay);

	cairo_move_to(overlay,x1,y1+sy); cairo_line_to(overlay,x2,y1+sy); cairo_stroke(overlay);
	cairo_move_to(overlay,x1,y1+2*sy); cairo_line_to(overlay,x2,y1+2*sy); cairo_stroke(overlay);


	cairo_set_font_size(overlay,sy/2);
	cairo_text_extents_t extents;
	cairo_text_extents (overlay, "0", &extents);
	x1-=(extents.width/2 + extents.x_bearing);
	y1-=(extents.height/2 + extents.y_bearing);

	cairo_set_source_rgba(overlay,1,0,0,0.2);

	cairo_move_to(overlay,x1+sx*0.5,y1+sy*0.5); cairo_show_text(overlay,"1");
	cairo_move_to(overlay,x1+sx*1.5,y1+sy*0.5); cairo_show_text(overlay,"2");
	cairo_move_to(overlay,x1+sx*2.5,y1+sy*0.5); cairo_show_text(overlay,"3");

	cairo_move_to(overlay,x1+sx*0.5,y1+sy*1.5); cairo_show_text(overlay,"4");
	cairo_move_to(overlay,x1+sx*1.5,y1+sy*1.5); cairo_show_text(overlay,"5");
	cairo_move_to(overlay,x1+sx*2.5,y1+sy*1.5); cairo_show_text(overlay,"6");

	cairo_move_to(overlay,x1+sx*0.5,y1+sy*2.5); cairo_show_text(overlay,"7");
	cairo_move_to(overlay,x1+sx*1.5,y1+sy*2.5); cairo_show_text(overlay,"8");
	cairo_move_to(overlay,x1+sx*2.5,y1+sy*2.5); cairo_show_text(overlay,"9");

	cairo_restore(overlay);
}

void draw_tag(cairo_t *c, char *id, struct tag *t);
void draw_wire(cairo_t *c, struct wire *w);

void Sidle(char);

void Swire(char c) {
	cairo_move_to(overlay,100,100);
	cairo_show_text(overlay,"WIRE");

	static int ts=0, ps=-1;

	char buf[32];
	switch(c) {
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		ts*=10; ts+=c-'0';
		if(ts<tagn) {
			cairo_save(overlay);
			cairo_set_source_rgba(overlay,1,0,0,0.8);
			cairo_rectangle(overlay,tag[ts].x,tag[ts].y,100,100);
			cairo_fill(overlay);

			if(ps>=0) {
				struct wire w={{ps,0},{ts,0}};
				draw_wire(overlay,&w);
				snprintf(buf,sizeof(buf),"%u - %u",ps,ts);
			} else {
				snprintf(buf,sizeof(buf),"%u",ts);
			}
			cairo_move_to(overlay,500,100);
			cairo_show_text(overlay,buf);

			cairo_restore(overlay);
		}
		break;
	case '-':
		if(ps>=0) addwire(ps,0,ts,0);
		ps=ts; ts=0;
		snprintf(buf,sizeof(buf),"%u - ",ps);
		cairo_move_to(overlay,500,100);
		cairo_show_text(overlay,buf);
		break;
	case '\r':
		if(ps>=0) addwire(ps,0,ts,0);
	case 27:
		Sidle(0); break;
	case 0:
		ts=0; ps=-1;
		state=WIRE;
	}
	
}

void Stag(char c) {
	cairo_move_to(overlay,100,100);
	cairo_show_text(overlay,"TAG");

	static int x=0,y=0,step=0;
	static struct tag t;

	int sx,sy;
	int dx=0,dy=0;

	switch(c) {
	case '0': break;
	case '1': dx=0; dy=0; goto update_grid;
	case '2': dx=1; dy=0; goto update_grid;
	case '3': dx=2; dy=0; goto update_grid;
	case '4': dx=0; dy=1; goto update_grid;
	case '5': dx=1; dy=1; goto update_grid;
	case '6': dx=2; dy=1; goto update_grid;
	case '7': dx=0; dy=2; goto update_grid;
	case '8': dx=1; dy=2; goto update_grid;
	case '9': dx=2; dy=2; goto update_grid;

	update_grid:
		grid_step(step,&sx,&sy);
		x+=sx*dx;
		y+=sy*dy;

		t.x=x; t.y=y; t.p=package;
		draw_tag(overlay,0,&t);

		grid_step(++step,&sx,&sy);
		target_grid(x,y,sx,sy);
		break;
	case '\r':
		addtag(x,y,0);
	case 27:
		Sidle(0); break;
	case 0:
		state=TAG;
		x=y=step=0;
		grid_step(step,&sx,&sy);
		target_grid(x,y,sx,sy);
	}
}


void Sidle(char c) {
	switch(c) {
	case 'o': Stag(0); break;
	case 'w': Swire(0); break;
	case 'u': state=UNWIRE; break;
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		state=EDIT; break;
	case 0:
		state=IDLE;
	}
}

int handle_key(char c) {
	cairo_save(overlay);
	cairo_set_source_rgba(overlay,1,1,1,0);
	cairo_set_operator(overlay,CAIRO_OPERATOR_SOURCE);
	cairo_paint(overlay);
	cairo_restore(overlay);
	cairo_set_source_rgb(overlay,0,0,0);

	switch(state) {
	case IDLE:
		if(c=='q') return 1;
		Sidle(c); break;
	case TAG:	Stag(c); break;
	case EDIT:	//Sedit(c); break;
	case WIRE:	Swire(c); break;
	case UNWIRE:	//Sunwire(c); break;
		break;
	}
	win_draw();
	return 0;
}

/*
 * Draw
 */

void draw_tag(cairo_t *c, char *id, struct tag *t) {
	t->p->draw(c,t->x,t->y);
	if(id) {
		cairo_move_to(c,t->x,t->y);
		cairo_show_text(c,id);
	}
}

void draw_wire(cairo_t *c, struct wire *w) {
	struct tag *a=tag+w->a.tag;
	struct tag *b=tag+w->b.tag;

	int ax,ay,bx,by;
	a->p->pin(w->a.pin,&ax,&ay);
	b->p->pin(w->b.pin,&bx,&by);

	cairo_save(c);
	cairo_set_line_width(c,30);
	cairo_move_to(c,a->x+ax,a->y+ay);
	cairo_line_to(c,b->x+bx,b->y+by);
	cairo_stroke(c);
	cairo_restore(c);
}

void draw(cairo_t *c) {
	int i;
	for(i=0;i<tagn;i++) {
		struct tag *t=&tag[i];
		char buf[256];
		snprintf(buf,sizeof(buf),"%u",i);
		draw_tag(c,buf,t);
	}

	for(i=0;i<wiren;i++) {
		draw_wire(c,wire+i);
	}
}


static void
win_handle_events(win_t *win)
{
    XEvent xev;

    while (1) {
	XNextEvent(win->dpy, &xev);
	switch(xev.type) {
	case KeyPress:
	{
	    XKeyEvent *kev = &xev.xkey;
	    
	    unsigned char keybuf[32];
	    XComposeStatus state;

	    XLookupString(kev,(char*)keybuf,sizeof(keybuf),NULL, &state);
	    if(handle_key(keybuf[0])) return;
	}
	break;
	case ConfigureNotify:
	{
	    XConfigureEvent *cev = &xev.xconfigure;

	    win->width = cev->width;
	    win->height = cev->height;
	    recreate_overlay();
	}
	break;
	case Expose:
	{
	    XExposeEvent *eev = &xev.xexpose;

	    if (eev->count == 0)
		win_draw(win);
	}
	break;
	}
    }
}

