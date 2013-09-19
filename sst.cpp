#include "ui.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>


int main(int argc, char* argv[]){
    //initialize timer for New York
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

    //load stock objects and timestamp
    StockList stockList(symbols);
    timer.timeStamp();

    //initialize ncurses
    if (startUI() == false){
        return 1;
    }
    //key handling variables
    bool exit = false;
    int key;
    
    //mainScreen variables
    int selected = -1;
    size_t offset=0;
    size_t end = stockList.size();

    //addStock variables
    std::string newStocks;

    //control flow and universal variables
    size_t menuHeight = 2;
    uint_fast8_t mode = 2;

    while (mode != 0){
        timeout(1);
        if (mode == 1){
            mode = mainScreen(stockList, timer, selected, offset, end, menuHeight);
        }
        if (mode == 2){
            mode = addStocks(stockList, newStocks);
        }
    }
    endwin();
    return(0);
}
