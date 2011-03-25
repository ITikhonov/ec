
struct package *package(char *);
int package_pin_rect(struct package *,int,int *x, int *y, int *w, int *h);
int package_line(struct package *,unsigned int,int *x0,int *y0);

void package_name(struct package *,char name[32]);

enum { PL_END=0,PL_MOVE,PL_LINE,PL_CLOSE };

