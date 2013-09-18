#include <ncurses.h>
#include <iostream>

bool startUI(){
    initscr();
    if (has_colors()){
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_BLACK, COLOR_RED);
        init_pair(5, COLOR_BLACK, COLOR_GREEN);
        init_pair(6, COLOR_BLACK, COLOR_YELLOW);
        init_pair(7, COLOR_WHITE, COLOR_BLUE);
        init_pair(8, COLOR_WHITE, COLOR_CYAN);
        timeout(5000);
        noecho();
        keypad(stdscr, TRUE);
        curs_set(0);
        return true;
    }
    else{
        endwin();
        std::cout << "Terminal does not support color." << std::endl;
        return false;
    }
}

