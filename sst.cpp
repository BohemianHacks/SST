#include "yfinance.h"
#include "ui.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>


int main(int argc, char* argv[]){

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
    size_t offset=0;
    size_t end = stockList.size();
    size_t menuHeight = 2;

    while (!exit){
        exit = mainScreen(stockList, selected, offset, end, menuHeight);
    }
    endwin();
    return(0);
}
