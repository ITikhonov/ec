#include "common.h"

/******************************************************
 * example: SO.6
 ******************************************************
 */

struct pk {
	struct package p;
	int pins;
};

NAME
	snprintf(name,32,"SO.%u",p->pins);
}

PIN
	if(n>p->pins) return 0;

	n--;
	int right=n/(p->pins/2);

	if(!right) {
		*x=0;
		*y=127*n+35;
		*h=38;
		*w=104;
	} else {
		*x=390+104;
		*y=35+127*(p->pins-n-1);
		*h=38;
		*w=104;
	}

	return 1;
}

LINE
	int D=(p->pins/2-1)*127+38+35*2;
	switch(n) {
	case 0: *x=104; *y=0; return PL_MOVE;
	case 1: *x=494; *y=0; return PL_LINE;
	case 2: *x=494; *y=D; return PL_LINE;
	case 3: *x=104; *y=D; return PL_LINE;
	case 4: return PL_CLOSE;
	case 5: *x=140; *y=35; return PL_MOVE;
	case 6: *x=140+38; *y=35; return PL_LINE;
	case 7: *x=140+38; *y=35+38; return PL_LINE;
	case 8: *x=140; *y=35+38; return PL_LINE;
	case 9: return PL_CLOSE;
	default: return PL_END;
	}
}


INIT
	p->pins=atoi(args);
END

