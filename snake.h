#include <stdbool.h>

void init();
void start();
void listen();
void colorfy();
void border_check();
void draw();
void step();
void turn(int dir);
void cslog();
void make_goal();
bool is_eat();
void eat();

enum Dir {
    up , right , down , left
};

struct Vec;

struct Pos {
    int y;
    int x;
    int color;
};

struct Cell {
    enum Dir dir;
    struct Pos pos;
    struct Vec * vec;
    struct Cell * next;
};

struct Vec {
    enum Dir dir;
    struct Pos pos;
    struct Vec * next;
};

extern struct Cell * head;
extern struct Cell * goal;