#include "sst.h"
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[]){
    //initialize timer and set timezone to New York
    setenv("TZ", "America/New_York", 1);
    BH::Timer timer;

    //create empty stock list and timestamp
    yfinance::StockList stockList;
    timer.timeStamp();
    //initialize ncurses
    if (startUI() == false){
        return 1;
    }
    const std::string prompt = "Enter symbols seperated by commas.";
    std::string symbols = textBox(prompt, prompt.length()+2);
    if (symbols.length() > 0){
        stockList.add(symbols);
    }
    else{
        endwin();
        std::cout << "No stocks given." << std::endl;
        return(1);
    }
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
            mode = mainScreen(stockList, timer, interval);
        }
        if (mode == 2){
            symbols = textBox(prompt, prompt.length()+2);
            if (symbols.length() > 0){
                stockList.add(symbols);
            }
            mode = 1;
        }
    }
    endwin();
    std::cout << yfinance::logging.str();
    return(0);
}
