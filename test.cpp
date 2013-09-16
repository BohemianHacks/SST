#include "yfinance.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){
    std::vector <std::string> symbols;
    std::vector <std::vector <std::string>> stockstrings;
    std::vector <Stock> stocks;
    std::ifstream list;
    list.open("stocks");
    if (list.is_open()){
        std::string sym;
        while (list >> sym){
            symbols.push_back(sym);
        }
    }
    loadStocks(symbols, stockstrings, stocks);
    for(size_t i = 0; i < stocks.size(); i++){
    std::cout << stocks[i].name << " " << stocks[i].current << std::endl;
    }
    return 0;
}
