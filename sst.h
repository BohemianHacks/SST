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
        init_pair(8, COLOR_BLACK, COLOR_CYAN);
        timeout(1);
        noecho();
        nonl();
        keypad(stdscr, true);
        curs_set(0);
        return true;
    }
    else{
        endwin();
        std::cout << "Terminal does not support color." << std::endl;
        return false;
    }
}

uint_fast8_t mainScreen(StockList& stockList, Timer& timer){
    
    int key;    
    size_t menuHeight = 2;
    int selected = -1;
    size_t offset=0;
    size_t end = stockList.size();
    uint_fast8_t mode = 1;
    short invert;
    size_t x, y;
    std::string& timeS = timer.stamp;
    
    while (mode == 1){
        //update screen size
        getmaxyx(stdscr, y, x);

        //key handling
        key = getch();
        if ((key == ERR) or (key == 'u')){
            if (stockList.update()){
                timer.timeStamp();
            }
        }
        if (key == 'q'){
            mode = 0;
            return(mode);
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
        timeout(1000);
    }
    return(mode);
}

uint_fast8_t addStocks(StockList& stockList, std::string& stocks){
    size_t x, y;
    timeout(-1);
    wchar_t key;
    getmaxyx(stdscr,y,x);
    std::string prompt = "Add stocks seperated by commas";
    const size_t width = (x-prompt.length()+2)/2;
    const size_t height = 4;
        
    erase();
    attron(COLOR_PAIR(8));
    mvprintw(y/2-height,width," %s ",prompt.c_str());
    attroff(COLOR_PAIR(8));
    attron(COLOR_PAIR(7));
    for (size_t i = 0; i < height; i++){
        mvprintw(y/2-i,width,"                                ");
    }
    attroff(COLOR_PAIR(7));
    attron(COLOR_PAIR(8));
    mvprintw(y/2-2,width+1,"%s",stocks.c_str());
    key = getch();
    attroff(COLOR_PAIR(8)); 
    if (key < KEY_MIN){
        stocks.push_back(char(key));
    }
    if ((key == KEY_BACKSPACE) and (stocks.length() > 0)){
        stocks.erase(stocks.length()-1,1);
    }
    if (key == 13){
        stockList.add(stocks);
        return(1);
    }
    return(2);
}
