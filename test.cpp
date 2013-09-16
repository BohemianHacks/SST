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
    if (getData(symbols, "nopl1", stockstrings)){
        for(size_t i = 0; i < stockstrings.size(); i++){
            Stock stock(symbols[i]);
            while(stockstrings[i][0].find('"') != -1 ){
                stockstrings[i][0].erase(stockstrings[i][0].find('"'),1);
            }
            stock.name = stockstrings[i][0];
            stock.open = (int_fast32_t)(100*atof(stockstrings[i][1].c_str()));
            stock.close = (int_fast32_t)(100*atof(stockstrings[i][2].c_str()));
            stock.current = (int_fast32_t)(100*atof(stockstrings[i][3].c_str()));
            stocks.push_back(stock);
        }

    }
    else{
        std::cout << "Failed to load stocks" << std::endl;
    }
    for(size_t i = 0; i < stocks.size(); i++){
    std::cout << stocks[i].name << " " << stocks[i].current << std::endl;
    }
    return 0;
}
