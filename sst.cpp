#include "sst.h"
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[]){
    //initialize timer and set timezone to New York
    setenv("TZ", "America/New_York", 1);
    BH::Timer timer;

    //initialize ncurses
    if (sst::startUI() == false){
        return 1;
    }
    const std::string prompt = "Enter symbols seperated by commas.";
    yfinance::StockList stockList(yfinance::splitCsv("amgn,rai,goog"));
    timer.timeStamp();

    if (stockList.size() < 1){
        endwin();
        std::cout << "No valid stocks given or unable to reach server." << std::endl;
        return(1);
    }
    
    //flow control
    uint_fast8_t mode = 1;
    
    //Interval to update stocks in milliseconds
    int interval = 1000;

    while (mode != 0){

        timeout(1);

        if (mode == 1){
            mode = sst::mainScreen(stockList, timer, interval);
        }
        if (mode == 2){
            mode = 1;
        }
    }
    endwin();
    std::cout << yfinance::logging.str();
    return(0);
}
