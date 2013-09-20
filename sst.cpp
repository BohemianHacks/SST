#include "sst.h"
#include <iostream>
#include <fstream>
#include <iomanip>

int main(int argc, char* argv[]){
    //initialize timer and set timezone to New York
    setenv("TZ", "America/New_York", 1);
    Timer timer;

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

    //load stock objects and timestamp
    StockList stockList(symbols);
    timer.timeStamp();
    stockList.add("amgn,yhoo");
    //initialize ncurses
    if (startUI() == false){
        return 1;
    }
    
    //addStock variables
    std::string newStocks;

    //flow control
    uint_fast8_t mode = 1;
    
    //Interval to update stocks in milliseconds
    int interval = 1000;

    while (mode != 0){

        timeout(1);

        if (mode == 1){
            mode = mainScreen(stockList, timer, interval);
        }
        if (mode == 2){

        }
    }
    endwin();
    return(0);
}
