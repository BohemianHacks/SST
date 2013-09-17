#include "yfinance.h"
#include "timer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <ncurses.h>

int main(int argc, char* argv[]){
    //Initialize Timer for New York
    Timer timer("America/New_York");

    //intialize stock holders
    std::vector <std::string> symbols;
    std::vector <Stock> stocks;
    std::ifstream list;

    //load stock symbols from list
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
    loadStocks(symbols, stocks);

    //initialize ncurses
    initscr();

    //print stock info
    printw("Last Updated: %s EST\n", timer.timeStamp().c_str());
    for(size_t i = 0; i < stocks.size(); i++){
        printw("%17s %7.2f %6.2f%%\n", stocks[i].getName().c_str(), stocks[i].getCurrent(), stocks[i].getChange());
    }

    refresh();
    getch();
    endwin();
    return(0);
}
