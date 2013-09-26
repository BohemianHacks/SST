#include <iostream>
#include <algorithm>
#include "yfinance.h"

int main(int argc, char* argv[]){
    yfinance::init();
    std::string layout = "open,close,last trade price,name,symbol,ask,average volume,volume,bid,book value per share,change,%change,year high,year low,change year high,change year low,%change year high,%change year low,year target price,day high,day low,dividend,%dividend,diluted eps,eps current year,eps next quarter,eps next year,peg,pe,pe rt,price book,ebitda,dividend pay date,market cap,market cap rt,currency,last trade time,ex dividend date,last trade date,revenue,exchange";
    std::string symbols = "bac,tsla,fish,flws,rai";
    yfinance::Ticker ticker(layout,symbols);
    std::cout << ticker.header() << std::endl;
    for (size_t i = 0; i < ticker.size(); i++){
        std::cout << ticker[i] << std::endl;
    }
    return(0);
}

