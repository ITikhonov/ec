
struct package {
        int (*pin_rect)(struct package *,int,int *x, int *y, int *w, int *h);
        void (*name)(struct package *,char name[32]);
        int (*line)(struct package *,unsigned int,int *x0,int *y0);
};

