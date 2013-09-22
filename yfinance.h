#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <curl/curl.h>

namespace yfinance{

const std::string BASE_URL = "http://download.finance.yahoo.com/d/quotes.csv?s=";
std::stringstream logging;

class Stock{
	friend class StockList;
    private:
        int_fast32_t close; //Yesterdays closing price
        int_fast32_t current; //Current price
        double change; //Percent change between current and close
        std::string symbol; //Ticker symbol
        std::string name;
    public:
        Stock(const std::string& sym):symbol(sym),color(0){};
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
    	//adds new stocks by csv string of ticker symbols. returns true if all stocks added
        bool add(const std::string SYMBOLS);
        //deletes stocks by same method. returns true if all stocks found and deleted
        bool del(const std::string SYMBOLS);
        //returns index of stock found by symbol
        size_t findStock(const std::string SYMBOL);
        bool update(); //updates current price, %change, and color
        size_t size(){return stocks.size();}; //Number of stocks contained
        Stock& stock(const size_t index){return stocks[index];};
        StockList();
};

//callback function for curl that writes text to readBuffer
static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);

//Scrape text from the web and return it to a string
bool getPage(const char* URL, std::string& readBuffer);

}
