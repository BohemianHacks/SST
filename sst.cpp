#include "yfinance.h"
#include "timer.h"
#include "ui.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>


int main(int argc, char* argv[]){
    //Initialize Timer for New York
    Timer timer("America/New_York");

    //load stock symbols from list
    std::ifstream list;
    std::vector <std::string> symbols;
    list.open("stocks");
    if (list.is_open()){
        std::string sym;
        while (list >> sym){
            symbols.push_back(sym);
        }
    }
    else{
        std::cout << "Could not load stocks file." << std::endl;
        return 1;
    }
    list.close();

    //load stock objects into vector
    StockList stockList(symbols);

    //initialize ncurses
    if (startUI() == false){
        return 1;
    }
    //key handling variables
    bool exit = false;
    int key;
    int selected = -1;
    short invert = 0;
    size_t offset=0;
    size_t end = stockList.size();
    size_t x, y;
    size_t menuHeight = 2;
    std::string timeS = timer.timeStamp();
    while (!exit){
        //update screen size
        getmaxyx(stdscr, y, x);

        //key handling
        key = getch();
        if ((key == ERR) or (key == 'u')){
            stockList.updateStocks();
            timeS = timer.timeStamp();
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
            attron(COLOR_PAIR(stockList.getStock(i).color+invert));
            printw("%17s %7.2f %7.2f%%\n", stockList.getStock(i).getName().c_str(), stockList.getStock(i).getCurrent(), stockList.getStock(i).getChange());
            attroff(COLOR_PAIR(stockList.getStock(i).color+invert));
        }

        //print menu
        attron(COLOR_PAIR(7));
        printw("(Q)uit (S)ort (A)dd stock (D)elete");
        attroff(COLOR_PAIR(7));

        refresh();
    }
    endwin();
    return(0);
}
