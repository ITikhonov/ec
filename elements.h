
struct element *element(unsigned int);
int element_x(struct element *);
int element_y(struct element *);
int element_f(struct element *);

char *element_name(struct element *);

struct element *element_find(char*);
struct element *element_create(char*);
void element_setx(struct element *,int);
void element_sety(struct element *,int);
void element_seta(struct element *,int);
void element_setflip(struct element *,int);
void element_set_package(struct element *,char *);

int pin_center(struct element *,int,int *x,int *y);
int pin_rect(struct element *,int,int x[4],int y[4]);

int body_line(struct element *,unsigned int,int *x0,int *y0);


#include <stdio.h>

void elements_save(FILE *);
void elements_load(FILE *);

