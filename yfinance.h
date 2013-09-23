#ifndef BH_YFINANCE_H
#define BH_YFINANCE_H

#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
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
    	std::map <std::string,int_fast32_t> numbers;
    	std::map <std::string,std::string> strings;
    	int_fast32_t volume;
    	int_fast32_t avgVol;
    public:
    	Stock(){};
        std::string get(std::string);
};

class StockList{
    private:
        std::map <std::string,Stock> stocks;
        std::vector <std::string> symbols;
        std::vector <std::string> properties;
    public:
    	//adds new stocks in vector of ticker symbols.
        void add(const std::vector <std::string>& SYMBOLS);
        //deletes stocks by same method.
        void del(const std::vector <std::string>& SYMBOLS);
        bool update(); //updates current price, %change, and color
        size_t size(){return stocks.size();}; //Number of stocks contained
        Stock operator[](std::string symbol){return(stocks[symbol]);};
        StockList(const std::vector <std::string>& SYMBOLS, const std::vector <std::string>& PROPERTIES);
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
