#include <iostream>
#include <algorithm>
#include "yfinance.h"

int main(){
    yfinance::init();
    yfinance::Ticker ticker("name|17,last trade price|7,%change|6","tsla,rai,goog,bac,fish,shp.l");
    for (size_t i = 0; i < ticker.size(); i++){
        std::cout << ticker[i] << std::endl;
    }
    return(0);
}

