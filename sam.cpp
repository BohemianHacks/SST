#include <iostream>
#include <algorithm>
#include "yfinance.h"

int main(int argc, char* argv[]){
    yfinance::init();
    std::string layout = "name,currency,open,last trade price,ask,bid,average volume,volume, %change,dividend,revenue,close";
    std::string symbols = "trx,iag,bbry,into,ea,sgamf,segavsmd,ubqu,athn,usas,dji,brk.a,tza,faz,ibm";
    yfinance::Ticker ticker(layout,symbols);
    std::cout << ticker.header() << std::endl;
    for (size_t i = 0; i < ticker.size(); i++){
        std::cout << ticker[i] << std::endl;
    }
    return(0);
}

