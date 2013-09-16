#include "yfinance.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <ncurses.h>

std::string timeStamp(){
    time_t rawtime;
    struct tm * ptm;
    time (&rawtime);
    ptm = localtime( &rawtime );
    char tStamp[50];
    sprintf(tStamp, "%02d-%02d-%4d %2d:%02d EST", ptm->tm_mon+1, ptm->tm_mday, ptm->tm_year+1900, (ptm->tm_hour)%24, ptm->tm_min);
    return tStamp;
}

int main(int argc, char* argv[]){
    //Set timezone to new york
    setenv("TZ", "America/New_York", 1);

    //intialize stock holders
    std::vector <std::string> symbols;
    std::vector <Stock> stocks;
    std::ifstream list;

    //load stocks
    list.open("stocks");
    if (list.is_open()){
        std::string sym;
        while (list >> sym){
            symbols.push_back(sym);
        }
    }
    else{
        std::cout << "Could not load stocks file." << std::endl;
        return 1;
    }
    list.close()

    //load stock objects
    loadStocks(symbols, stocks);

    //write screen
    std::cout << "Last Updated: " << timeStamp() << std::endl;
    for(size_t i = 0; i < stocks.size(); i++){
        std::cout << std::setw(17) << stocks[i].getName();
        printf(" %7.2f %6.2f%% \n", stocks[i].getCurrent(), stocks[i].getChange());
    }
    return(0);
}
