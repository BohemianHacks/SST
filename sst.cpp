#include "sst.h"
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[]){
    //initialize timer and set timezone to New York
    setenv("TZ", "America/New_York", 1);
    Timer timer;

    //create empty stock list and timestamp
    std::vector <std::string> symbols;
    StockList stockList(symbols);
    timer.timeStamp();
    //initialize ncurses
    if (startUI() == false){
        return 1;
    }

    stockList.add(textBox("Enter symbols seperated by commas.",30));    
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
