#include <ncurses.h>

bool curses_started = false;

void endCurses() {
   if (curses_started && !isendwin())
           endwin();
}

void startCurses() {
    if (curses_started) {
        refresh();
    }
    else {
        initscr();
        cbreak();
        noecho();
        intrflush(stdscr, false);
        keypad(stdscr, true);
        atexit(endCurses);
        curses_started = true;
    }
}

int main(){
    startCurses();
    while(1){
        char a = getch();
        
        if(a =='a'){
            addstr("The 'a' key was pressed.\n");
        }
    }
}
