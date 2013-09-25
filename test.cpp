#include <iostream>
#include <algorithm>
#include "yfinance.h"

int main(int argc, char* argv[]){
    yfinance::init();
    std::string layout = "exchange|8,symbol|5,last trade price|7,%change|6,pe|7";
    std::string symbols = "tsla,rai,goog,bac,fish,shp.l,yhoo";
    yfinance::Ticker ticker(layout,symbols);
    for (size_t i = 0; i < ticker.size(); i++){
        std::cout << ticker[i] << std::endl;
    }
    return(0);
}

