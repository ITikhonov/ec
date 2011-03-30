#include "common.h"

/******************************************************
 * Rectangular example: R.0805
 ******************************************************
 */

struct pk {
	struct package p;
	int size;
};

NAME
	snprintf(name,32,"R.%u",p->size);
}

PIN
	if(n>2 || n<1) return 0;
	switch(p->size) {
	case 805:
		if(n==1) { *x=0; *y=0; *w=40; *h=125; }
		else if(n==2) { *x=160; *y=0; *w=40; *h=125; }
		break;
	default:
		return 0;
	}

	return 1;
}

LINE
	switch(n) {
	case 0: *x=40; *y=0; return PL_MOVE;
	case 1: *x=160; *y=0; return PL_LINE;
	case 2: *x=40; *y=125; return PL_MOVE;
	case 3: *x=160; *y=125; return PL_LINE;

	case 4: return PL_CLOSE;
	case 5: *x=70; *y=20; return PL_MOVE;
	case 6: *x=70; *y=100; return PL_LINE;
	default: return PL_END;
	}
}

INIT
	p->size=atoi(args);
END

