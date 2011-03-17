struct element;

void make_wire(struct element *,int,struct element *,int);

struct wire *wire(unsigned int i);

struct element *wire_a(struct wire *,int *);
struct element *wire_b(struct wire *,int *);

