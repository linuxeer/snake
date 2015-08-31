#include <ncurses.h>
#include "snake.h"

int main(int argc , char ** argv) {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr , TRUE);
    printw("welcome to play this game !");
    srand(time(NULL));
    colorfy();
    init();
    make_goal();
    start();
    listen();
    endwin();
}
