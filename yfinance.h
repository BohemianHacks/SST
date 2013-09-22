#include <curl/curl.h>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <vector>
#include <cstdlib>

namespace yfinance{

const std::string BASE_URL = "http://download.finance.yahoo.com/d/quotes.csv?s=";
std::stringstream logging;
typedef std::vector <std::string> line;

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
        std::vector <Stock> stocks;
        line symbols;
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
static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp){
            ((std::string*)userp)->append((char*)contents, size * nmemb);
                        return size * nmemb;
}

//Scrape text from the web and return it to a string
bool getPage(const char* URL, std::string& readBuffer){
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl){ 
		curl_easy_setopt(curl, CURLOPT_URL, URL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK){
            		curl_easy_cleanup(curl);
            		logging << "yfinance::getPage() Could not fetch URL: " << URL << std::endl;
			return false;
		}
        curl_easy_cleanup(curl);
	return true;
	}
	curl_easy_cleanup(curl);
	return false;
}

StockList::StockList(){};

}
