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
#include <iomanip>
#include <algorithm>
#include <curl/curl.h>

namespace yfinance{

const std::string BASE_URL = "http://download.finance.yahoo.com/d/quotes.csv?s=";

class Stock{
	friend class StockList;
    private:
    	std::map <std::string,int_fast32_t> numbers;
    	std::map <std::string,std::string> strings;
    public:
    	Stock(){};
        std::string operator[](std::string);
};

class StockList{
	friend class Ticker;
	friend StockSort;
    private:
        std::map <std::string,Stock> stocks;
        std::vector <std::string> symbols;
        std::vector <std::string> properties;
    public:
    	StockList(){};
    	//adds new stocks in vector of ticker symbols.
        void addStocks(const std::vector <std::string>& SYMBOLS);
        //deletes stocks by same method.
        void delStocks(const std::vector <std::string>& SYMBOLS);
        bool update(); //updates all stocks contained
        size_t size(){return(stocks.size());}; //Number of stocks contained
        Stock operator[](std::string symbol){return(stocks[symbol]);};
        bool setProperties(std::vector <std::string>& newProperties);
        bool addProperties(std::vector <std::string>& newProperties);
        bool delProperties(std::vector <std::string>& newProperties);
        bool sort(std::string property);
};

class Ticker{
    private:
        StockList stockList;
        std::vector <std::string> properties;
        std::string sortProperty;
    public:
        Ticker(std::string& layout, std::string& symbolString);
        std::string header();
        std::string operator[](size_t index);
        bool setSort(std::string newSort);
        size_t size(){return(stockList.size());};
        uint_fast16_t interval; //update interval in tenths of a second
        
};

struct stockProperty{
    std::string label;
    std::string urlCode;
    uint_fast8_t width;
};

struct StockSort{
    StockSort(StockList sList, std::string property) : property(property), sList(sList){};
    bool operator()(std::string symbol1, std::string symbol2);
    private:
        std::string property;
        StockList sList;
};

//initialize variables
void init();

//create properties list from formatted string
std::vector <std::string> parseProperties(std::string propertyString);

//create valid property list
bool cleanProperties(std::vector <std::string>& properties);

//create format from property list
std::string createFormat(const std::vector <std::string>& properties);

//split a line of csv text into a string vector
std::vector <std::string> splitCsv(std::string csvLine);

//callback function for curl that writes text to readBuffer
static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);

//Scrape data from yahoo finance and return it to a string
bool getData(const std::vector <std::string>& symbols, const std::string& format, std::string& readBuffer);

std::string trim(const std::string& str);

}

#endif
