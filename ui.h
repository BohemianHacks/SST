#include <ncurses.h>
#include <iostream>
#include "yfinance.h"
#include "timer.h"

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

bool mainScreen(StockList& stockList, Timer& timer, int& selected, size_t& offset, size_t& end, const size_t& menuHeight){
    int key = getch();
    bool exit = false;
    short invert;
    size_t x, y;
    std::string& timeS = timer.timeStamp();
    
    //update screen size
    getmaxyx(stdscr, y, x);

    //key handling
    if ((key == ERR) or (key == 'u')){
        stockList.update();
        timer.timeStamp();
    }
    if (key == 'q'){
        exit = true;
    }
    if (key == KEY_UP){
        if (selected > -1){
            selected--;
        }
        if ((selected - offset == -1) and (offset > 0)){
            offset--;
        }
    }
    if (key == KEY_DOWN){
        if (selected < int(stockList.size())-1){
            selected++;
        }
        if ((selected >= y + offset - menuHeight) and (selected < stockList.size())){
            offset++;
        }
    }
    if (offset + y - menuHeight > stockList.size()){
        end = stockList.size();
    }
    else{
        end = offset + y - menuHeight;
    }

    //Erase screen buffer
    erase();

    //print stock info
    printw("Last Updated: %s EST\n", timeS.c_str());
    for(size_t i = offset; i < end; i++){
        if (i == selected){
            invert = 3;
        }
        else{
            invert = 0;
        }
        attron(COLOR_PAIR(stockList.stock(i).color+invert));
        printw("%17s %7.2f %7.2f%%\n", stockList.stock(i).getName().c_str(), stockList.stock(i).getCurrent(), stockList.stock(i).getChange());
        attroff(COLOR_PAIR(stockList.stock(i).color+invert));
    }
    //print menu
    attron(COLOR_PAIR(7));
    printw("(Q)uit (S)ort (A)dd stock (D)elete");
    attroff(COLOR_PAIR(7));
    refresh();
    return exit;
}

