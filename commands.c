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

#include "elements.h"

static struct element *e=0;

static void select_element(char *s) {
	e=element_find(s);
	if(!e) e=element_create(s);
}

static void pos(char *s) {
	char sx[8]={0},sy[8]={0},sa[8]={0},f=0;
	sscanf(s,"%[^.].%[^.].%[^.].%c",sx,sy,sa,&f);
	if(*sx) { element_setx(e,atoi(sx)); }
	if(*sy) { element_sety(e,atoi(sy)); }
	if(*sa) { element_seta(e,atoi(sa)); }
	if(f) { element_setflip(e,f=='f'); }
}

static void package(char *s) {
	element_set_package(e,s);
}


void command(char *s) {
	switch(*s) {
	case 'A'...'Z': select_element(s); break;
	case '@': pos(s+1); break;
	case '#': package(s+1); break;
	//case '.': pin(s+1); break;
	//case '=': name(s+1); break;
	//case '&': part(s+1); break;
	}
}

