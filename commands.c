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
#include <string.h>

#include "commands.h"
#include "elements.h"
#include "wires.h"

static char we[32];
static int wp=-1;

static char e[32];
static int spin=-1;

void select_element(char *s) {
	element_create(s);
	strcpy(e,s);
}

static void package(char *s) {
	element_set_package(e,s);
}

static void pin(char *s) {
	spin=atoi(s);
	if(we[0]&&wp!=-1) {
		make_wire(we,wp,e,spin);
		we[0]=0; wp=0;
	}
}

static void wiring() {
	strcpy(we,e);
	wp=spin;
}

static int command(char *s);

void load_schem(char *s) {
        char buf[1024],*p=0;
        FILE *f=fopen(s,"r");
        for(;;) {
                int c=fgetc(f);
                switch(c) {
                case '[': p=buf; continue;
                case ']': if(p) { *p++=0; command(buf); p=0; } continue;
                case ' ': if(p) { *p++=0; command(buf); p=buf; } continue;
                case EOF: goto exit;
                default: if(p) *p++=c;
                }
        }
        exit:;
}

static int command(char *s) {
	printf("command '%s'\n",s);
	switch(*s) {
	case 'A'...'Z': select_element(s); break;
	case '#': package(s+1); break;
	case '.': pin(s+1); break;
	case '-': wiring(); break;
	case 'c': load_schem(s+1); break;
	}
	return 0;
}

