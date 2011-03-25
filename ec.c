#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <cairo.h>
#include <cairo-xlib.h>

#include <X11/Xutil.h>

#include "commands.h"

extern void draw(cairo_t *cr);

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

cairo_surface_t *overlay_s=0;
cairo_t *overlay=0;

char cmd[255],*pcmd=cmd;

static void win_draw()
{
    cairo_surface_t *surface;
    cairo_t *cr;
    Visual *visual = DefaultVisual(win.dpy, DefaultScreen (win.dpy));

    XClearWindow(win.dpy, win.win);

    surface=cairo_xlib_surface_create(win.dpy, win.win, visual,
					 win.width, win.height);


    cairo_surface_t *buf=cairo_image_surface_create(CAIRO_FORMAT_RGB24,win.width,win.height);

    cr = cairo_create(buf);
    cairo_set_source_rgb(cr,1,1,1);
    cairo_paint(cr);

    cairo_set_source_rgb(cr,0,0,0);
    cairo_set_font_size (cr,10);

    draw(cr);

    //cairo_set_source_surface(cr,overlay_s,0,0);
    //cairo_paint (cr);

    cairo_set_source_rgb(cr,0,0,0);
    cairo_move_to(cr,10,10);
    *pcmd=0;
    cairo_show_text(cr,cmd);
    printf("%s\n",cmd);


    cairo_destroy(cr);
    cairo_surface_flush(buf);

    cr = cairo_create(surface);
    cairo_set_source_surface(cr,buf,0,0);
    cairo_paint(cr);

    if (cairo_status (cr)) {
	printf("Cairo is unhappy: %s\n",
	       cairo_status_to_string (cairo_status (cr)));
	exit(0);
    }

    cairo_destroy(cr);
    cairo_surface_destroy (buf);
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
		 |ButtonReleaseMask
		 |StructureNotifyMask
		 |ExposureMask);

    XMapWindow(win->dpy, win->win);
}

static void
win_deinit(win_t *win)
{
    XDestroyWindow(win->dpy, win->win);
}


void clear_overlay() {
	cairo_save(overlay);
	cairo_set_source_rgba(overlay,1,1,1,0);
	cairo_set_operator(overlay,CAIRO_OPERATOR_SOURCE);
	cairo_paint(overlay);
	cairo_restore(overlay);
	cairo_set_source_rgb(overlay,0,0,0);
}

void handle_button(int x,int y,int button) {
	char buf[1024];
	char c=(button==Button1)?'@':'!';
	sprintf(buf,"%c%d0.%d0",c,x,y);
	command(buf);
	win_draw();
}

int handle_key(char c) {
	printf("%x (%c)\n",c,c);
	switch(c) {
	case '\0': return 0;
	case '\x08':
		if(pcmd>cmd) *(--pcmd)=0;
		break;
	case '\r':
	case ' ':
		*pcmd=0;
		if(command(cmd)) return 1;
		*(pcmd=cmd)=0;
		break;
	default:
		*pcmd++=c;
	} 
	win_draw();
	return 0;
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
	case ButtonRelease:
	{
	    XButtonEvent *bev = &xev.xbutton;
	    handle_button(bev->x,bev->y,bev->button);
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

