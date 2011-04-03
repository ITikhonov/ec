struct board_wire *board_wire(unsigned int i);
struct board_wire *board_find_wire(char *a,int ap,char *b,int bp);
struct board_wire *board_make_wire(char *a,int ap,char *b,int bp);

int board_add_corner(struct board_wire *w,int after,int x,int y);
int board_corner(struct board_wire *w,unsigned int i,int *x,int *y);
void board_corner_move(struct board_wire *w,int n,int x,int y);
struct board_wire *board_pick_corner(int x,int y,int *no,int nth);

char *board_wire_a(struct board_wire *w);
char *board_wire_b(struct board_wire *w);
int board_wire_ap(struct board_wire *w);
int board_wire_bp(struct board_wire *w);

#include <stdio.h>

void board_wires_load(FILE *f);
void board_wires_save(FILE *f);
