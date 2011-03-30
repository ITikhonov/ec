#include "common.h"

struct pk {
	struct package p;
	int size;
};

NAME
	snprintf(name,32,"R.%u",p->size);
}

PIN
	if(n>5 || n<1) return 0;

	// middle = 290/2
	// offmiddle = 190/2

	*w=50; *h=35;
	switch(n) {
	case 1: *x=0; *y=290/2-190/2-35/2; break;
	case 2: *x=0; *y=290/2+190/2-35/2; break;

	case 3: *x=215; *y=290/2-190/2-35/2; break;
	case 4: *x=215; *y=290/2-35/2; break;
	case 5: *x=215; *y=290/2+190/2-35/2; break;
	default: return 0;
	}
	return 1;
}


LINE
	switch(n) {
	case 0: *x=50; *y=0; return PL_MOVE;
	case 1: *x=215; *y=0; return PL_LINE;
	case 2: *x=215; *y=290; return PL_LINE;
	case 3: *x=50; *y=290; return PL_LINE;
	case 4: return PL_CLOSE;
	default: return PL_END;
	}
}

INIT
END

