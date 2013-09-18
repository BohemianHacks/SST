#include <curl/curl.h>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <vector>
#define baseURL "http://download.finance.yahoo.com/d/quotes.csv?s="

class Stock{
	friend bool loadStocks(const std::vector <std::string>& symbols, std::vector <Stock>& stocks);
    friend bool updateStocks(std::vector <std::string>& symbols, std::vector <Stock>& stocks);
    protected:
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
        short color;
        std::string getSymbol(){return symbol;};
        std::string getName(){return name;};
};

//callback function for curl
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
            ((std::string*)userp)->append((char*)contents, size * nmemb);
                        return size * nmemb;
}

//Scrape text from the web and return it to a string
bool getPage(const char* url, std::string& readBuffer){
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl){ 
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK){
			return false;
		}
		return true;
	}
	curl_easy_cleanup(curl);
}

//Get single stock's current price
bool getPrice(const std::string& symbol, int_fast32_t& price){
    std::stringstream urlBuilder;
    std::string response;
    urlBuilder << baseURL << symbol << "&f=l1";
    for (uint_fast8_t i = 0; i < 3; i++){
        if (getPage(urlBuilder.str().c_str(),response)){
            price = int(100*atof(response.c_str()));
        }
        if (price > 0){
            return true;
        }
    }
    return false;
}

//Get multiple stocks and values from one csv, then split them into string vectors
bool getData(const std::vector <std::string>& symbols, const std::string& format, std::vector <std::vector <std::string>>& stocks){
    std::stringstream urlBuilder;
    std::stringstream syms;
    for (size_t i = 0; i < symbols.size()-1; i++){
        syms << symbols[i] << ",";
    }
    syms << symbols[symbols.size()-1];
    std::string response;
    urlBuilder << baseURL << syms.str() << "&f=" << format;
    for (uint_fast8_t i = 0; i < 3; i++){
        if (getPage(urlBuilder.str().c_str(),response)){
            std::stringstream resp;
            std::string line;
            resp << response;
            while(std::getline(resp, line)){
                std::vector <std::string> stock;
                std::stringstream l;
                std::string value;

                l << line;
                while(std::getline(l,value,',')){
                    stock.push_back(value);
                }
                stocks.push_back(stock);
            }
            if (stocks.size() == symbols.size()){
                return true;
            }
            return false;
        }
    }
    return false;
}

//Update list of stocks
bool updateStocks(std::vector <std::string>& symbols,std::vector <Stock>& stocks){
    std::vector <std::vector <std::string>> data;
    if (getData(symbols, "l1", data)){
        for (size_t i = 0; i < stocks.size(); i++){
            stocks[i].current = (int_fast32_t)(100*atof(data[i][0].c_str()));
            stocks[i].change = 100.0*(double(stocks[i].current - stocks[i].close)/double(stocks[i].close));
            if (stocks[i].change > 0.0){
                stocks[i].color = 2;
            }
            else if (stocks[i].change == 0.0){
                stocks[i].color = 3;
            }
            else{
                stocks[i].color = 1;
            }
        }
        return(true);
    }
}
//load a list of ticker symbols with name, close, current, and change into Stock vector
bool loadStocks(const std::vector <std::string>& symbols, std::vector <Stock>& stocks){
    std::vector <std::vector <std::string>> stockstrings;
    if (getData(symbols, "npl1", stockstrings)){
        for(size_t i = 0; i < stockstrings.size(); i++){
            Stock stock(symbols[i]);
            while(stockstrings[i][0].find('"') != -1 ){
                stockstrings[i][0].erase(stockstrings[i][0].find('"'),1);
            }
            stock.name = stockstrings[i][0];
            stock.close = (int_fast32_t)(100*atof(stockstrings[i][1].c_str()));
            stock.current = (int_fast32_t)(100*atof(stockstrings[i][2].c_str()));
            stock.change = 100.0*(double(stock.current-stock.close)/double(stock.close));
            if (stock.change > 0.0){
                stock.color = 2;
            }
            else if (stock.change == 0.0){
                stock.color = 3;
            }
            else{
                stock.color = 1;
            }
            stocks.push_back(stock);
        }
        return true;
    }
    	return false;
}
