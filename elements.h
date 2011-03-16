
struct element *element(unsigned int);
int element_x(struct element *);
int element_y(struct element *);

struct element *element_find(char*);
struct element *element_create(char*);
void element_setx(struct element *,int);
void element_sety(struct element *,int);
void element_seta(struct element *,int);
void element_setflip(struct element *,int);

