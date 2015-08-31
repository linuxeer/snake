#include "snake.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

struct Cell * head = NULL;
struct Cell * goal = NULL;

void init() {
    head = malloc(sizeof(struct Cell));
    head->pos = (struct Pos) {5 , 9 , 1};
    head->dir = right;
    head->vec = malloc(sizeof(struct Vec));
    head->vec->pos = (struct Pos) {-1 , -1 , 0};
    head->vec->dir = right;
    head->vec->next = NULL;

    struct Cell * c0 = malloc(sizeof(struct Cell));
    c0->pos = (struct Pos) {5 , 8 , 2};
    c0->dir = right;
    c0->vec = malloc(sizeof(struct Vec));
    c0->vec->pos = (struct Pos) {-1 , -1 , 0};
    c0->vec->dir = right;
    c0->vec->next = NULL;

    struct Cell * c1 = malloc(sizeof(struct Cell));
    c1->pos = (struct Pos) {5 , 7 , 3};
    c1->dir = right;
    c1->vec = malloc(sizeof(struct Vec));
    c1->vec->pos = (struct Pos) {-1 , -1 , 0};
    c1->vec->dir = right;
    c1->vec->next = NULL;

    struct Cell * c2 = malloc(sizeof(struct Cell));
    c2->pos = (struct Pos) {5 , 6 , 4};
    c2->dir = right;
    c2->vec = malloc(sizeof(struct Vec));
    c2->vec->pos = (struct Pos) {-1 , -1 , 0};
    c2->vec->dir = right;
    c2->vec->next = NULL;

    head->next = c0;
    c0->next = c1;
    c1->next = c2;
    c2->next = NULL;
}

void start() {
    step();
    signal(SIGALRM , start);
    alarm(1);
}

void listen() {
    int ch;
    while (true) {
        ch = getch();
        switch (ch) {
            case KEY_LEFT:
                turn(left);
                break;
            case KEY_RIGHT:
                turn(right);
                break;
            case KEY_UP:
                turn(up);
                break;
            case KEY_DOWN:
                turn(down);
                break;
            case 'q':
                return;
        }
    }
}

void turn(int dir) {
    struct Cell * iter = head;
    if (iter->dir == dir) {
        return;
    } else if (iter->dir == down && dir == up) {
        return;
    } else if (iter->dir == up && dir == down) {
        return;
    } else if (iter->dir == right && dir == left) {
        return;
    } else if (iter->dir == left && dir == right) {
        return;
    }
    int y = head->pos.y , x = head->pos.x;
    while (iter) {
        struct Vec * vp = iter->vec;
        while (vp->next) {
            vp = vp->next;
        }
        vp->next = malloc(sizeof(struct Vec));
        vp->next->dir = dir;
        vp->next->pos = (struct Pos) {y , x , 0};
        vp->next->next = NULL;
        iter = iter->next;
    }
}

void border_check() {
    int rows , cols;
    getmaxyx(stdscr , rows , cols);
    if (head->pos.x * 2 > cols || head->pos.x * 2 == 0 || head->pos.y > rows - 1) {
        exit(1);
    }
}

void colorfy() {
    start_color();
    init_pair(1 , COLOR_RED , COLOR_YELLOW);
    init_pair(2 , COLOR_RED , COLOR_RED);
    init_pair(3 , COLOR_RED , COLOR_BLUE);
    init_pair(4 , COLOR_RED , COLOR_CYAN);
}

void draw() {
    clear();
    struct Cell * iter = head;
    while (iter) {
        attron(COLOR_PAIR(iter->pos.color));
        mvprintw(iter->pos.y , iter->pos.x * 2 , "  ");
        attroff(COLOR_PAIR(iter->pos.color));
        iter = iter->next;
    }
    if (goal) {
        attron(COLOR_PAIR(goal->pos.color));
        mvprintw(goal->pos.y , goal->pos.x * 2 , "  ");
        attroff(COLOR_PAIR(goal->pos.color));
    }
    refresh();
    border_check();
}

void step() {
    struct Cell * iter = head;
    while (iter) {
        struct Vec * vp = iter->vec->next;
        if (vp && vp->pos.y == iter->pos.y && vp->pos.x == iter->pos.x) {
            iter->dir = vp->dir;
            iter->vec->next = vp->next;
        }
        switch (iter->dir) {
            case up:
                iter->pos.y--;
                break;
            case down:
                iter->pos.y++;
                break;
            case left:
                iter->pos.x--;
                break;
            case right:
                iter->pos.x++;
                break;
        }
        iter = iter->next;
    }
    if (is_eat()) {
        eat();
        make_goal();
    }
    draw();
}

bool is_eat() {
    switch (head->dir) {
        case down:
            if (head->pos.y + 1 == goal->pos.y && head->pos.x == goal->pos.x) {
                return true;
            }
            break;
        case up:
            if (head->pos.y - 1 == goal->pos.y && head->pos.x == goal->pos.x) {
                return true;
            }
            break;
        case left:
            if (head->pos.y == goal->pos.y && head->pos.x - 1 == goal->pos.x) {
                return true;
            }
            break;
        case right:
            if (head->pos.y == goal->pos.y && head->pos.x + 1 == goal->pos.x) {
                return true;
            }
            break;
    }
    return false;
}

void eat() {
    goal->dir = head->dir;
    goal->vec = malloc(sizeof(struct Vec));
    goal->vec->pos = (struct Pos) {-1 , -1 , 0};
    goal->vec->dir = right;
    goal->vec->next = NULL;
    goal->next = head;
    head = goal;
}

void make_goal() {
    static int rows , cols;
    getmaxyx(stdscr , rows , cols);
    int goal_y = rand() % (rows);
    int goal_x = rand() % (cols / 2);
    goal = malloc(sizeof(struct Cell));
    goal->pos = (struct Pos) {goal_y , goal_x , rand() % 4 + 1};
    goal->dir = -1;
    goal->vec = NULL;
    goal->next = NULL;
}