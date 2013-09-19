#include <curl/curl.h>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <vector>

const std::string BASE_URL = "http://download.finance.yahoo.com/d/quotes.csv?s=";

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
        short color;
        std::string getSymbol(){return symbol;};
        std::string getName(){return name;};
};

class StockList{
    protected:
        std::vector <Stock> stocks;
        std::vector <std::string> symbols;
    public:
        bool add(std::string symbol);
        bool remove(std::string symbol);
        bool update();
        size_t size(){return stocks.size();};
        Stock& stock(const size_t index){return stocks[index];};
        StockList(const std::vector <std::string>& SYMBOLS);
};

//turn csv string into multidimensional vector
std::vector <std::vector <std::string> > csvStringVector(std::string csv){
    std::stringstream csvStream(csv);
    std::string line;
    std::vector <std::vector <std::string> > csvVector;
    while(std::getline(csvStream, line)){
        std::vector <std::string> lineVector;
        lineVector.clear();
        while(line.find(',') != -1){
            std::string value;
            size_t qPos1 = line.find('"');
            size_t qPos2 = line.find('"', qPos1+1);
            size_t cPos1 = line.find(',');
            size_t cPos2 = line.find(',',cPos1+1);


            if ((qPos1 < cPos1) && (cPos1 < qPos2) && (qPos2 != -1)){
                value = line.substr(qPos1, qPos2+1);
                if (cPos2 != -1){
                    line.erase(0,qPos2+2);
                }
                else{
                    line.erase(0, line.length());
                }
            }
            else{
                value = line.substr(0, cPos1);
                line.erase(0, cPos1+1);
            }
            lineVector.push_back(value);
        }
        lineVector.push_back(line);
        csvVector.push_back(lineVector);
    }
    return(csvVector);
}

//callback function for curl
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
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
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK){
            curl_easy_cleanup(curl);
			return false;
		}
        curl_easy_cleanup(curl);
		return true;
	}
	curl_easy_cleanup(curl);
}

//Get multiple stocks and values from one csv, then split them into string vectors
bool getData(const std::vector <std::string>& SYMBOLS, const std::string& FORMAT, std::vector <std::vector <std::string>>& data){
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
		data = csvStringVector(response);
            if (data.size() == SYMBOLS.size()){
                return true;
            }
            return false;
        }
    }
    return false;
}

//load a list of ticker symbols with name, close, current, and change into Stock vector
StockList::StockList(const std::vector <std::string>& SYMBOLS){
    std::vector <std::vector <std::string>> stockstrings;
    if (getData(SYMBOLS, "npl1", stockstrings)){
        for(size_t i = 0; i < stockstrings.size(); i++){
            Stock stock(SYMBOLS[i]);
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
        symbols = SYMBOLS;
    }
}

StockList::add(const std::string SYMBOLS){
    std::vector <std::vector <std::string>> stockstrings;
    std::vector <std::string> syms;
    std::stringstream l(SYMBOLS); 
    while(std::getline(l,value,',')){
        syms.push_back(value);
    }
    if (getData(syms, "npl1", stockstrings)){
        for(size_t i = 0; i < stockstrings.size(); i++){
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
        }
        symbols.insert(symbols.end(),syms.begin(),syms.end());
    }
}

//Update list of stocks
bool StockList::update(){
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
    return(false);
}
