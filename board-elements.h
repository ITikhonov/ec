struct board_element *board_element(unsigned int i);

int board_element_x(struct board_element *e);
int board_element_y(struct board_element *e);
int board_element_f(struct board_element *e);
int board_element_h(struct board_element *e);
char *board_element_name(struct board_element *e);

struct board_element *board_element_find(char *s);
struct board_element *board_element_create(char *s);

void board_element_setx(struct board_element *e,int x);
void board_element_sety(struct board_element *e,int y);
void board_element_seta(struct board_element *e,int a);
void board_element_setf(struct board_element *e,int f);
void board_element_seth(struct board_element *e,int h);

void board_pin_rect(struct board_element *e,int x[4],int y[4]);
void board_pin_center(struct board_element *e,int x[4],int y[4]);
void board_body_line(struct board_element *e,int *x,int *y);

#include <stdio.h>

void board_elements_save(FILE *f);
void board_elements_load(FILE *f);
