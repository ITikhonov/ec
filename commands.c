/*
 * 
 * MCU #TQFP44 @200.200.45.f &PIC18F4550 .9 =AN0
 * element "MCU", set package to TQFP44, place at 200:200 angle 45 flipped, part PIC18F4550, name pin 9 "AN0"
 * 
 * R? $1M @100.200..n #0805
 * element "R1" (increase number if clash), 1M value, place at @100:200 not flipped preserve angle, set package to #0850
 * 
 * 
 * MCU .AN0 R? #100K @300.300 #0805 .1 .2 AMP .OUT
 * pin AN0 of MCU connect to newly created element Rx on first pin, configure R?, then wire MCU.AN0 to Rx.1, Rx.2 to AMP.OUT
 * 
 * state: element selected, pin selected
 *
 * MCU - select element "MCU", create if needed.
 * #TQFP.44 - set package to TQFP with 44 pins
 * @200.200.45.f - set position, angle and if flipped
 * &PIC18F4550 - set part number, contains pin names
 * .9 - select pin in selected element by number
 * .AN0 - .. by name
 * =AN0 - add name to selected pin
 * 
 * If pin is being selected having other pin selected wire them together.
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "commands.h"
#include "elements.h"
#include "wires.h"

static struct element *we=0;
static int wp=-1;


static struct element *e=0;
static int spin=-1;

static struct wire *w;
static int wc=-1;
static int wnth=0;

struct element *selected(int *p) {
	*p=spin;
	return e;
}

struct wire *selected_wire_corner(int *n) {
	*n=wc;
	return w;
}

static void select_element(char *s) {
	e=element_find(s);
	if(!e) e=element_create(s);
	spin=-1;
}


static void pos(char *s) {
	char sx[8]={0},sy[8]={0},sa[8]={0},f=0;
	sscanf(s,"%[^.].%[^.].%[^.].%c",sx,sy,sa,&f);

	if(e) {
		if(*sx) { element_setx(e,atoi(sx)); }
		if(*sy) { element_sety(e,atoi(sy)); }
		if(*sa) { element_seta(e,atoi(sa)); }
		if(f) { element_setflip(e,f=='f'); }
	} else {
		w=pick_wire_corner(atoi(sx),atoi(sy),&wc,wnth++);
		if(w==0) {
			wnth=0;
			w=pick_wire_corner(atoi(sx),atoi(sy),&wc,wnth++);
		}
	}
}

static void package(char *s) {
	element_set_package(e,s);
}

static void pin(char *s) {
	spin=atoi(s);
	int t;
	if(!pin_rect(e,spin,&t,&t,&t,&t)) spin=-1;
	else if(we&&wp!=-1) {
		make_wire(we,wp,e,spin);
		we=e; wp=spin;
	}
}


static void wiring() {
	we=e;
	wp=spin;
}



static void save(char *s) {
	FILE *f=fopen(s,"w");
	elements_save(f);
	fclose(f);
}

static void load(char *s) {
	FILE *f=fopen(s,"r");
	elements_load(f);
	wires_load(f);
	fclose(f);
}

static void wireadjust() {
	e=0; we=0;
}

static void move(char *s) {
	int x,y;
	sscanf(s,"%d.%d",&x,&y);
	if(w && wc>=0) {
		wire_corner_move(w,wc,x,y);
	}
}

static void add() {
	if(w) {
		int x,y,x1,y1;
		if(wire_corner(w,wc,&x,&y)) {
			if(!wire_corner(w,wc+1,&x1,&y1)) {
				wire_corner(w,wc-1,&x1,&y1);
			}
			x=(x+x1)/2;
			y=(y+y1)/2;

			wc=wire_add_corner(w,wc,x,y);
		}
	}
}


int command(char *s) {
	printf("command '%s'\n",s);
	switch(*s) {
	case '\0': add(); break;
	case 'A'...'Z': select_element(s); break;
	case '@': pos(s+1); break;
	case '!': move(s+1); break;
	case '#': package(s+1); break;
	case '.': pin(s+1); break;
	case '-': wiring(); break;
	case 'w': wireadjust(); break;
	case 's': save(s+1); break;
	case 'l': load(s+1); break;
	case 'q': return 1;
	//case '=': name(s+1); break;
	//case '&': part(s+1); break;
	}
	return 0;
}

