#include <iostream>
#include <algorithm>
#include "yfinance.h"

int main(){
    yfinance::init();
    std::vector <std::string> properties = yfinance::splitCsv("name,last trade price,%change");
    std::vector <std::string> symbols = yfinance::splitCsv("TSLA,RAI,BAC,SHP.L,YHOO,FISH,GOOG");
    yfinance::StockList stockList;
    stockList.setProperties(properties);
    stockList.addStocks(symbols);
    for (size_t i = 0; i < symbols.size(); i++){
        for (size_t j = 0; j < properties.size(); j++){
            printf("%s ",stockList[symbols[i]][properties[j]].c_str());
        }
        std::cout << std::endl;
    }
    return(0);
}

