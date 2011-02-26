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

int
main(int argc, char *argv[])
{

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

static void win_draw()
{
    cairo_surface_t *surface;
    cairo_t *cr;
    Visual *visual = DefaultVisual(win.dpy, DefaultScreen (win.dpy));

    XClearWindow(win.dpy, win.win);

    surface = cairo_xlib_surface_create (win.dpy, win.win, visual,
					 win.width, win.height);
    cr = cairo_create(surface);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_font_size (cr, 100);
    cairo_scale(cr,0.1,0.1);

    draw(cr);

    if (cairo_status (cr)) {
	printf("Cairo is unhappy: %s\n",
	       cairo_status_to_string (cairo_status (cr)));
	exit(0);
    }

    cairo_destroy(cr);
    cairo_surface_destroy (surface);
}

static void
win_init(win_t *win)
{
    Window root;

    win->width = 400;
    win->height = 400;

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

void Pt8(cairo_t *c, int x, int y) {
	cairo_save(c);

	cairo_set_source_rgb(c,0,0,0);
	cairo_rectangle(c,x+100,y+100,700,700);
	cairo_stroke(c);

	cairo_set_source_rgb(c,1,0,0);
	int i;

	for(i=0;i<8;i++) { cairo_rectangle(c,x,y+80*i+140,100,37); cairo_fill(c); }
	for(i=0;i<8;i++) { cairo_rectangle(c,x+800,y+80*i+140,100,37); cairo_fill(c); }
	for(i=0;i<8;i++) { cairo_rectangle(c,x+80*i+140,y,37,100); cairo_fill(c); }
	for(i=0;i<8;i++) { cairo_rectangle(c,x+80*i+140,y+800,37,100); cairo_fill(c); }

	cairo_restore(c);
}

/*
 * Data: tags
 */

struct tag {
	int x,y; // 0.01mm units
	void (*p)(cairo_t *,int,int);
} tag[1024]={{1000,1000,Pt8}};
int tagn=1;

void addtag(int x,int y) {
	tag[tagn].x=x;
	tag[tagn].y=x;
	tag[tagn].p=Pt8;
	tagn++;
}


/*
 * o66/72 - put new tag at 66x72
 * 67pt8 - assign tag #67 to be in a package TQFP-32 (7x7mm lead: 1mm length 0.8mm space 0.37mm width)
 * w67/8-o8745-69/2-72 - wire from pin 8 of #67 to 87x45 to pin 2 of 69 to point 72
 * u67/8-o8745-69/2-72 - unwire from pin 8 of #67 to 87x45 to pin 2 of 69 to point 72
 * 67d - delete tag #67
 */

enum { IDLE, TAG, EDIT, WIRE, UNWIRE } state=IDLE;

void Sidle(c) {
	switch(c) {
	case 'o': state=TAG; break;
	case 'w': state=WIRE; break;
	case 'u': state=UNWIRE; break;
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		state=EDIT; break;
	}
}

void Stag(char c) {
	static enum { X,Y } st=X;
	static int x=0,y=0;
	switch(c) {
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		if(st==X) { x*=10; x+=c-'0'; } else { y*=10; y+=c-'0'; }
		break;
	case '/':
		if(st==X) st=Y; break;
	case '\r':
		if(st==Y) { addtag(x,y); x=0; y=0; st=X; state=IDLE; } break;
	}
}

int handle_key(char c) {
	switch(state) {
	case IDLE:
		if(c=='q') return 1;
		Sidle(c); break;
	case TAG:	Stag(c); break;
	//case EDIT:	Sedit(c); break;
	//case WIRE:	Swire(c); break;
	//case UNWIRE:	Sunwire(c); break;
	}
	win_draw();
	return 0;
}

/*
 * Draw
 */

void draw(cairo_t *cr) {
	int i;
	for(i=0;i<tagn;i++) {
		struct tag *t=&tag[i];
		t->p(cr,t->x,t->y);
		char buf[256];
		snprintf(buf,sizeof(buf),"%u",i);
		cairo_move_to(cr,t->x,t->y);
		cairo_show_text(cr,buf);
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

