#include <iostream>
#include <algorithm>
#include "yfinance.h"

int main(int argc, char* argv[]){
    yfinance::init();
    std::string layout = "name,currency,close,open,last trade price,ask,bid,year high,year low,year target price,average volume,volume,%change,dividend,%dividend,revenue,market cap,book";
    std::string symbols = "trx,iag,bbry,into,ea,sgamf,ubqu,athn,usas,dji,brk.a,tza,faz,ibm";
    yfinance::Ticker ticker(layout,symbols,"VOLUME");
    std::cout << ticker.header() << std::endl;
    for (size_t i = 0; i < ticker.size(); i++){
        std::cout << ticker[i] << std::endl;
    }
    return(0);
}

