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

std::string textBox(const std::string title, const size_t width){
    std::string userText;
    int key;
    timeout(100);
    while (key != 13){
        size_t x, y;
        getmaxyx(stdscr,y,x);
        size_t startX = (x-width)/2;
        size_t startY = y/2-4;
        std::string fill;
        fill.append(width,' ');
        
        erase();
        attron(COLOR_PAIR(7));
        for (size_t i = 1; i < 4; i++){
            mvprintw(startY+i,startX,fill.c_str());
        }
        attroff(COLOR_PAIR(7));
        attron(COLOR_PAIR(8));
        mvprintw(startY,startX,fill.c_str());
        fill.erase(0,2);
        mvprintw(startY, (x-title.length())/2,title.c_str());
        mvprintw(startY+2, startX+1, fill.c_str());
        mvprintw(startY+2, startX+1, userText.c_str());
        attroff(COLOR_PAIR(8));
        refresh();

        key = getch();
        if (userText.length() < width-2 && ((('A' <= key) && (key <= 'Z')) 
                    || (('a' <= key) && (key <='z'))
                    || (key == ',') || (key == '.'))){
            userText.append(1,char(key));
        }
        if ((key == KEY_BACKSPACE) && (userText.length() > 0)){
            userText.erase(userText.length()-1,1);
        }
    }
    return(userText); 
}

uint_fast8_t mainScreen(yfinance::StockList& stockList, Timer& timer, int& interval){
    
    int key; //Hold key codes for processing
    size_t menuHeight = 2; //Lines to leave for extra info
    int selected = -1; //Which stock is selected. -1 is no stock
    size_t offset=0; //Number of lines to offset for viewing in a smaller window
    size_t end = stockList.size(); //Where to stop printing stocks to avoid over filling screen
    uint_fast8_t mode = 1; //Flow control variable returned on mode change
    short invert; //Each stocks color(1-3) has an inverse(4-6). When selected we invert the colors.
    size_t x, y; //size of the screen
    std::string& timeS = timer.stamp; //reference to latest time stamp string
    
    while (mode == 1){
        //update screen size
        getmaxyx(stdscr, y, x);

        //key handling
        key = getch();
        
        //'u' Updates prices, %change, color, and time stamp. We also update if getch() times out.
        if ((key == ERR) or (key == 'u')){
            if (stockList.update()){
                timer.timeStamp();
            }
        }
        //'q' exits program
        if (key == 'q'){
            mode = 0;
            return(mode);
        }
        if (key == 'a'){
            mode = 2;
            return(mode);
        }
        //arrow keys control what is selected
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
        
        //Make sure we dont overfill the screen or request something past the end of the stock list
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
        
        //print menu keys
        attron(COLOR_PAIR(7));
        printw("(Q)uit (S)ort (U)pdate (A)dd (D)elete (H)elp");
        attroff(COLOR_PAIR(7));
        refresh();
        timeout(interval);
    }
    return(mode);
}
