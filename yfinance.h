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
        std::string getSymbol(){return symbol;};
        std::string getName(){return name;};
        short color; //1 = red. 2 = green. 3 = yellow.
};

class StockList{
    protected:
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

//turn csv string into multidimensional vector
std::vector <line> csvLineVector(std::string csv){
    std::stringstream csvStream(csv);
    std::string csvLine;
    std::vector <line> csvVector;
    while(std::getline(csvStream, csvLine)){
        line lineVector;
        lineVector.clear();
        while(csvLine.find(',') != -1){
            std::string value;
            size_t qPos1 = csvLine.find('"');
            size_t qPos2 = csvLine.find('"', qPos1+1);
            size_t cPos1 = csvLine.find(',');
            size_t cPos2 = csvLine.find(',',cPos1+1);
		
            //Check for comma inside quotes
            if ((qPos1 < cPos1) && (cPos1 < qPos2) && (qPos2 != -1)){
                value = csvLine.substr(qPos1, qPos2+1);
                if (cPos2 != -1){
                    csvLine.erase(0,qPos2+2);
                }
                else{
                    csvLine.erase(0, csvLine.length());
                }
            }
            else{
                value = csvLine.substr(0, cPos1);
                csvLine.erase(0, cPos1+1);
            }
            if (value.length() > 0){
                lineVector.push_back(value);
            }
        }
        if (csvLine.length() > 0){
            lineVector.push_back(csvLine);
        }
        csvVector.push_back(lineVector);
    }
    return(csvVector);
}

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

//Get multiple stocks and values from one csv, then split them into string vectors
//FORMAT is a string of tags found here https://code.google.com/p/yahoo-finance-managed/wiki/enumQuoteProperty
bool getData(const line& SYMBOLS, const std::string& FORMAT, std::vector <line>& data){
    std::stringstream urlBuilder;
    std::stringstream syms;
    for (size_t i = 0; i < SYMBOLS.size()-1; i++){
        syms << SYMBOLS[i] << ",";
    }
    syms << SYMBOLS[SYMBOLS.size()-1];
    std::string response;
    urlBuilder << BASE_URL << syms.str() << "&f=" << FORMAT;
    for (uint_fast8_t i = 0; i < 3; i++){
        if (getPage(urlBuilder.str().c_str(),response)){
		data = csvLineVector(response);
            if (data.size() == SYMBOLS.size()){
                return true;
            }
            logging << "yfinance::getData() Mismatch in number of symbols and data returned." << std::endl;
            return false;
        }
    }
    logging << "yfinance::getData() Could not fetch URL: " << urlBuilder.str() << std::endl;
    return false;
}

StockList::StockList(){};

bool StockList::add(const std::string SYMBOLS){
    std::vector <line> stockstrings = csvLineVector(SYMBOLS);
    line syms = stockstrings[0];
    stockstrings.clear();
    std::stringstream duplicateStocks;
    std::stringstream invalidStocks;
    std::stringstream validStocks;
    if (getData(syms, "npl1", stockstrings)){
        for(size_t i = 0; i < stockstrings.size(); i++){
            if ((findStock(syms[i]) == -1) && (stockstrings[i][1].compare("N/A") != 0)){
                Stock stock(syms[i]);
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
                symbols.push_back(syms[i]);
                validStocks << syms[i] << " ";
            }
            else if (findStock(syms[i]) != -1){
            	duplicateStocks << syms[i] << " ";
            }
            else if (stockstrings[i][1].compare("N/A") == 0){
            	invalidStocks << syms[i] << " ";
            }
        }
        if (validStocks.str().length() > 0)
        	logging << "Added stocks: " << validStocks.str() << std::endl;
        if (duplicateStocks.str().length() > 0)
        	logging << "Duplicate stocks: " << duplicateStocks.str() << std::endl;
        if (invalidStocks.str().length() > 0)
        	logging << "Invalid stocks: " << invalidStocks.str() << std::endl;
        return(true);
    }
    if (validStocks.str().length() > 0)
    	logging << "Added stocks: " << validStocks.str() << std::endl;
    if (duplicateStocks.str().length() > 0)
    	logging << "Duplicate stocks: " << duplicateStocks.str() << std::endl;
    if (invalidStocks.str().length() > 0)
    	logging << "Invalid stocks: " << invalidStocks.str() << std::endl;
    return(false);
}

bool StockList::del(const std::string SYMBOLS){
	std::vector <line> symbolList = csvLineVector(SYMBOLS);
	std::stringstream deletedStocks;
	std::stringstream unfoundStocks;
	for (size_t i = 0; i < symbolList[0].size(); i++){
		if(findStock(symbolList[0][i]) != -1){
			stocks.erase(stocks.begin() + i);
			symbols.erase(symbols.begin() + i);
			deletedStocks << symbolList[0][i] << " ";
		}
		else{
			unfoundStocks << symbolList[0][i] << " ";
		}
	}
	if (deletedStocks.str().length() > 0)
		logging << "Deleted Stocks: " << deletedStocks.str() << std::endl;
	if (unfoundStocks.str().length() > 0)
		logging << "Could not find: " << unfoundStocks.str() << std::endl;
}
	
		

size_t StockList::findStock(const std::string SYMBOL){
    for (size_t i = 0; i < stocks.size(); i++){
        if (stocks[i].symbol == SYMBOL){
            return(i);
        }
    }
    return(size_t(-1));
}

//Update list of stocks
bool StockList::update(){
    std::vector <line> data;
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
    logging << "Could not update stocks." << std::endl;
    return(false);
}

}
