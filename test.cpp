#include <iostream>
#include <algorithm>
#include "yfinance.h"

int main(){
    yfinance::init();
    std::string layout = "name|17,last trade price|7,%change|6";
    std::string symbols = "tsla,rai,goog,bac,fish,shp.l,yhoo";
    yfinance::Ticker ticker(layout,symbols);
    for (size_t i = 0; i < ticker.size(); i++){
        std::cout << ticker[i] << std::endl;
    }
    return(0);
}

