#include <iostream>
#include <algorithm>
#include "yfinance.h"

int main(){
    yfinance::init();
    std::vector <std::string> symbols = yfinance::splitCsv("TSLA,RAI,BAC,SHP.L,YHOO,FISH,GOOG");
    yfinance::Ticker ticker("name|17,last trade price|7,%change|6",symbols);
    for (size_t i = 0; i < ticker.size(); i++){
        std::cout << ticker[i] << std::endl;
    }
    return(0);
}

