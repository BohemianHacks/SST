#ifndef BH_YFINANCE_H
#define BH_YFINANCE_H

#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <curl/curl.h>

namespace yfinance{

const std::string BASE_URL = "http://download.finance.yahoo.com/d/quotes.csv?s=";

class Stock{
	friend class StockList;
    private:
        int_fast32_t close; //Yesterdays closing price
        int_fast32_t current; //Current price
        double change; //Percent change between current and close
        std::string symbol; //Ticker symbol
        std::string name;
    public:
        Stock(){};
        float getClose(){return close/100.0;};
        float getCurrent(){return current/100.0;};
        float getChange(){return change;};
        std::string getSymbol(){return symbol;};
        std::string getName(){return name;};
        short color; //1 = red. 2 = green. 3 = yellow.
};

class StockList{
    private:
        std::map <std::string,Stock> stocks;
        std::vector <std::string> symbols;
    public:
    	//adds new stocks in vector of ticker symbols.
        void add(std::vector <std::string>& SYMBOLS, std::vector <std::string>& properties);
        //deletes stocks by same method.
        void del(const std::vector <std::string>& SYMBOLS);
        bool update(); //updates current price, %change, and color
        size_t size(){return stocks.size();}; //Number of stocks contained
        Stock operator[](std::string symbol){return(stocks[symbol]);};
        StockList(std::vector <std::string>& SYMBOLS, std::vector <std::string>& properties);
};

//initialize variables
void init();

//create format from property list
std::string createFormat(std::vector <std::string>& properties);

//split a line of csv text into a string vector
std::vector <std::string> splitCsv(std::string csvLine);

//callback function for curl that writes text to readBuffer
static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);

//Scrape data from yahoo finance and return it to a string
bool getData(const std::vector <std::string>& symbols, const std::string& format, std::string& readBuffer);

}

#endif
