#include "yfinance.h"

namespace yfinance{

StockList::StockList(const std::vector <std::string>& SYMBOLS):symbols(SYMBOLS){
    std::string rawData;
    if (getData(SYMBOLS,"s0n0pl1",rawData)){
    	std::stringstream csvStream(rawData);
    	std::string csvLine;
        while(std::getline(csvStream, csvLine)){
            std::vector <std::string> data = splitCsv(csvLine);
            if (data[2] != "N/A"){
            	Stock stock(data[0]);
            	stock.name = data[1];
            	stock.close = (int_fast32_t)(100*atof(data[2].c_str()));
            	stock.current = (int_fast32_t)(100*atof(data[3].c_str()));
            	stocks[data[0]] = stock;
            }
        }
    }
	
}

std::vector <std::string> splitCsv(std::string csvLine){
    std::string value;
    std::vector <std::string> lineVector;
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
        lineVector.push_back(value);
    }
}

static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp){
            ((std::string*)userp)->append((char*)contents, size * nmemb);
                        return size * nmemb;
}

bool getData(const std::vector <std::string>& SYMBOLS, const std::string& format, std::string& readBuffer){
	std::stringstream urlBuilder(BASE_URL);
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
