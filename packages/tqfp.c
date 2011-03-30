#include "common.h"

/******************************************************
 * TQFP.44
 ******************************************************
 */

struct pk {
	struct package p;
	int pins;
};

NAME
	snprintf(name,32,"TQFP.%u",p->pins);
}

PIN
	if(n>p->pins) return 0;

	n-=1;
	int side=n/(p->pins/4);
	switch(side) {
	case 0: // Left
		*x=0;
		*y=n*80+181;
		*w=100;
		*h=37;
		return 1;
	case 1: // Bottom
		*x=(n-11)*80+181;
		*y=1100;
		*w=37;
		*h=100;
		return 1;
	case 2: // Right
		*x=1100;
		*y=1200-181-(n-22)*80;
		*w=100;
		*h=-37;
		return 1;
	case 3: // Top
		*x=1200-181-(n-33)*80;
		*y=0;
		*w=-37;
		*h=100;
		return 1;
	}
	return 0;
}

LINE
	switch(n) {
	case 0: *x=100; *y=100; return PL_MOVE;
	case 1: *x=1100; *y=100; return PL_LINE;
	case 2: *x=1100; *y=1100; return PL_LINE;
	case 3: *x=100; *y=1100; return PL_LINE;
	case 4: return PL_CLOSE;
	case 5: *x=140; *y=181; return PL_MOVE;
	case 6: *x=140; *y=181+37; return PL_LINE;
	default: return PL_END;
	}
}

INIT
	p->pins=atoi(args);
END

