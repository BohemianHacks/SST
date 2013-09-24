#include "yfinance.h"
#include <iostream>

namespace yfinance{

std::stringstream logging;
std::map <std::string,std::string> numberProperties;
std::map <std::string,std::string> stringProperties;

std::string Stock::operator[](std::string property){
    std::ostringstream value;
    value.precision(2);
    if (numberProperties.count(property) == 1){
    	if (property == "VOLUME"){
    	    value << std::fixed << volume;
    	}
    	else if (property == "AVERAGE_VOL"){
    	    value << std::fixed << avgVol;
    	}
    	else{
    	    value << std::fixed << numbers[property]/100.0;
    	}
    }
    else if (stringProperties.count(property) == 1){
    	value << strings[property];
    }
    return(value.str());
}
	
StockList::StockList(const std::vector <std::string>& SYMBOLS, const std::vector <std::string>& PROPERTIES){
	properties = PROPERTIES;
	add(SYMBOLS);
}

void StockList::add(const std::vector <std::string>& SYMBOLS){
    std::string rawData;
    if (getData(SYMBOLS,createFormat(properties),rawData)){
    	std::stringstream csvStream(rawData);
    	std::string csvLine;
        while(std::getline(csvStream, csvLine)){
            std::vector <std::string> data = splitCsv(csvLine);
            if (stocks.count(data[0]) == 0){
            	Stock stock;
            	for (size_t i = 0; i < properties.size(); i++){
            	    if (numberProperties.count(properties[i]) == 1){
            	    	if (properties[i] == "VOLUME"){
            	    	    stock.volume = (int_fast32_t)atoi(data[i+1].c_str());
            	    	}
            	    	else if (properties[i] == "AVERAGE_VOL"){
            	    	    stock.avgVol = (int_fast32_t)atoi(data[i+1].c_str());
            	    	}
            	    	else{
            	    	    stock.numbers[properties[i]] = (int_fast32_t)(100.0*atof(data[i+1].c_str()));
            	    	}
            	    }
            	    else if (stringProperties.count(properties[i]) == 1){
            	    	stock.strings[properties[i]] = data[i+1];
            	    }
            	}
            	stocks[data[0]] = stock;
            	symbols.push_back(data[0]);
            }
        }
    }
}

bool StockList::update(){
    std::string rawData;
    if (getData(symbols,createFormat(properties),rawData)){
    	std::stringstream csvStream(rawData);
    	std::string csvLine;
        while(std::getline(csvStream, csvLine)){
            std::vector <std::string> data = splitCsv(csvLine);
            for (size_t i = 0; i < properties.size(); i++){
                if (numberProperties.count(properties[i]) == 1){
               	    if (properties[i] == "VOLUME"){
            	        stocks[data[0]].volume = (int_fast32_t)atoi(data[i+1].c_str());
            	    }
            	    else if (properties[i] == "AVERAGE_VOL"){
            	        stocks[data[0]].avgVol = (int_fast32_t)atoi(data[i+1].c_str());
            	    }
            	    else{
            	        stocks[data[0]].numbers[properties[i]] = (int_fast32_t)(100.0*atof(data[i+1].c_str()));
            	    }
            	}
            	else if (stringProperties.count(properties[i]) == 1){
            	    stocks[data[0]].strings[properties[i]] = data[i+1];
            	}
            }
        }
    }
}

void init(){
numberProperties["ASK"] = "b2";
numberProperties["ASK_SIZE"] = "a5";
numberProperties["AVERAGE_VOL"] = "a2";
numberProperties["VOLUME"] = "v0";
numberProperties["BID"] = "b3";
numberProperties["BID_SIZE"] = "b6";
numberProperties["BOOK_VALUE_PER_SHARE"] = "b4";
numberProperties["CHANGE"] = "c6";
numberProperties["CHANGE_PERCENT"] = "p2";
numberProperties["CHANGE_YEAR_HIGH"] = "k4";
numberProperties["CHANGE_YEAR_LOW"] = "j5";
numberProperties["DAY_HIGH"] = "h0";
numberProperties["DAY_LOW"] = "g0";
numberProperties["DIVIDEND_PAY_DATE"] = "r1";
numberProperties["TRAILING_DIVIDEND"] = "d0";
numberProperties["TRAILING_DIVIDEND_PERCENT"] = "y0";
numberProperties["DILUTED_EPS"] = "e0";
numberProperties["EBITDA"] = "j4";
numberProperties["EPS_EST_CURRENT_YEAR"] = "e7";
numberProperties["EPS_EST_NEXT_QTR"] = "e9";
numberProperties["EPS_EST_NEXT_YEAR"] = "e8";
numberProperties["LAST_TRADE_PRICE"] = "l1";
numberProperties["LAST_TRADE_SIZE"] = "k3";
numberProperties["YEAR_TARGET_PRICE"] = "t8";
numberProperties["OPEN"] = "o0";
numberProperties["PEG"] = "r5";
numberProperties["PE"] = "r0";
numberProperties["PE_RT"] = "h0";
numberProperties["PERCENT_CHANGE_YEAR_HIGH"] = "k5";
numberProperties["PERCENT_CHANGE_YEAR_LOW"] = "j6";
numberProperties["CLOSE"] = "p0";
numberProperties["PRICE_BOOK"] = "p6";
stringProperties["MARKET_CAP"] = "j1";
stringProperties["MARKET_CAP_RT"] = "j3";
stringProperties["CURRENCY"] = "c4";
stringProperties["LAST_TRADE_TIME"] = "t1";
stringProperties["EX_DIVIDEND_DATE"] = "q0";
stringProperties["LAST_TRADE_DATE"] = "d1";
stringProperties["REVENUE"] = "s6";
stringProperties["EXCHANGE"] = "x0";
stringProperties["NAME"] = "n0";
stringProperties["SYMBOL"] = "s0";
}

bool verifyProperties(std::vector <std::string>& properties){
    for (size_t i = 0; i < properties.size(); i++){
        std::transform(properties[i].begin(), properties[i].end(),properties[i].begin(), ::toupper);
        if ((numberProperties.count(properties[i]) == 0) && (stringProperties.count(properties[i]) == 0)){
            properties.erase(properties.begin()+i);
        }
    }
    if (properties.size() > 0){
        return(true);
    }
    return(false);
}

std::string createFormat(const std::vector <std::string>& properties){
    std::stringstream format;
    format << "s0";
    for (size_t i = 0; i < properties.size(); i++){
    	if (numberProperties.count(properties[i]) == 1){
    	    format << numberProperties[properties[i]];
    	}
    	else if (stringProperties.count(properties[i]) == 1){
    	    format << stringProperties[properties[i]];
    	}
    }
    return(format.str());
}

std::vector <std::string> splitCsv(std::string csvLine){
    std::string value;
    std::vector <std::string> lineVector;
    //while we have commas to split data, get each cell and push into a vector
    while(csvLine.find(',') != -1){
        std::string value;
        size_t qPos1 = csvLine.find('"');
        size_t qPos2 = csvLine.find('"', qPos1+1);
        size_t cPos1 = csvLine.find(',');
        size_t cPos2 = csvLine.find(',',cPos1+1);
		
        //Check for comma inside quotes
        if ((qPos1 < cPos1) && (cPos1 < qPos2) && (qPos2 != -1)){
            //add everything in the quotes to escape commas
            value = csvLine.substr(qPos1, qPos2+1);
            if (cPos2 != -1){
            	//erase the comma after the second quote
                csvLine.erase(0,qPos2+2);
            }
            else{
            	//if no second comma then the line is done
                csvLine.erase(0, csvLine.length());
            }
        }
        else{
            //add from the start to the first comma then clear it
            value = csvLine.substr(0, cPos1);
            csvLine.erase(0, cPos1+1);
        }
        //strip quotes and add to vector
        while (value.find('"') != -1){value.erase(value.find('"'),1);}
        lineVector.push_back(value);
    }
    //strip quotes and add to vector if anything is left
    if (csvLine.length() > 0){
        while (csvLine.find('"') != -1){csvLine.erase(csvLine.find('"'),1);}
        lineVector.push_back(csvLine);
    }
    return(lineVector);
}

static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp){
            ((std::string*)userp)->append((char*)contents, size * nmemb);
                        return size * nmemb;
}

bool getData(const std::vector <std::string>& SYMBOLS, const std::string& format, std::string& readBuffer){
	std::stringstream urlBuilder;
	urlBuilder << BASE_URL;
	for (size_t i = 0; i < SYMBOLS.size()-1; i++){
		urlBuilder << SYMBOLS[i] << ",";
	}
	urlBuilder << SYMBOLS[SYMBOLS.size()-1] << "&f=" << format;
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl){ 
		curl_easy_setopt(curl, CURLOPT_URL, urlBuilder.str().c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK){
            		curl_easy_cleanup(curl);
            		logging << "yfinance::getData() Could not fetch URL: " << urlBuilder.str() << std::endl;
			return false;
		}
        curl_easy_cleanup(curl);
	return true;
	}
	curl_easy_cleanup(curl);
	logging << "yfinance::getData() Could not fetch URL: " << urlBuilder.str() << std::endl;
	return false;
}

}
