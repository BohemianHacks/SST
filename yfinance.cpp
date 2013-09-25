#include "yfinance.h"
#include <iostream>

namespace yfinance{

std::stringstream logging;
std::map <std::string,stockProperty> numberProperties;
std::map <std::string,stockProperty> stringProperties;

std::string Stock::operator[](std::string property){
    std::ostringstream value;
    value.precision(2);
    value << std::fixed;
    if (numberProperties.count(property) == 1){
    	if (property.find("VOLUME") != -1){
    	    value << numbers[property];
    	}
    	else{
    	    value << std::fixed << numbers[property]/100.0;
    	}
    	if (property.find('%') != -1){
    	    value << '%';
    	}
    }
    else if (stringProperties.count(property) == 1){
    	value << strings[property];
    }
    return(value.str());
}

void StockList::addStocks(const std::vector <std::string>& SYMBOLS){
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
            	    	if (properties[i].find("VOLUME") != -1){
            	    	    stock.numbers[properties[i]] = (int_fast32_t)atoi(data[i+1].c_str());
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

void StockList::delStocks(const std::vector <std::string>& SYMBOLS){
    for (size_t i = 0; i < SYMBOLS.size(); i++){
        if ((stocks.count(SYMBOLS[i]) == 1) && (std::find(symbols.begin(), symbols.end(), SYMBOLS[i]) != symbols.end())){
            stocks.erase(SYMBOLS[i]);
            symbols.erase(std::find(symbols.begin(), symbols.end(), SYMBOLS[i]));
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
               	    if (properties[i].find("VOLUME") != -1){
            	        stocks[data[0]].numbers[properties[i]] = (int_fast32_t)atoi(data[i+1].c_str());
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
        return(true);
    }
    return(false);
}

bool StockList::setProperties(std::vector <std::string>& newProperties){
    if (cleanProperties(newProperties)){
	properties = newProperties;
	return(true);
    }
    return(false);
}

bool StockList::addProperties(std::vector <std::string>& newProperties){
    bool added = false;
    if (cleanProperties(newProperties)){
	for (size_t i = 0; i < newProperties.size(); i++){
	    if (std::find(properties.begin(), properties.end(), newProperties[i]) == properties.end()){
	    	properties.push_back(newProperties[i]);
	    	added = true;
	    }
	}
	return(added);
    }
    return(false);
}

bool StockList::delProperties(std::vector <std::string>& newProperties){
    bool deleted = false;
    if (cleanProperties(newProperties)){
	for (size_t i = 0; i < newProperties.size(); i++){
	    if (std::find(properties.begin(), properties.end(), newProperties[i]) != properties.end()){
	    	properties.erase(std::find(properties.begin(), properties.end(), newProperties[i]));
	    	deleted = true;
	    }
	}
	return(deleted);
    }
    return(false);
}

Ticker::Ticker(std::string& layout, std::string& symbolString){
    std::vector <std::string> symbols = splitCsv(symbolString);
    std::vector <std::string> splitLayout = splitCsv(layout);
    for (size_t i = 0; i < splitLayout.size(); i++){
    	std::string property = splitLayout[i];
    	std::transform(property.begin(), property.end(),property.begin(), ::toupper);
    	if ((numberProperties.count(property) == 1) || (stringProperties.count(property) == 1)){
    	    properties.push_back(property);
    	}
    }
    for (size_t i = 0; i < symbols.size(); i++){
    	std::transform(symbols[i].begin(), symbols[i].end(),symbols[i].begin(), ::toupper);
    }
    stockList.setProperties(properties);
    stockList.addStocks(symbols);
}

std::string Ticker::header(){
    std::ostringstream labels;
    for (size_t i = 0; i < properties.size(); i++){
    	int width = 0;
    	std::string lbl;
    	if (numberProperties.count(properties[i]) == 1){
    	    width = numberProperties[properties[i]].width;
    	    lbl = numberProperties[properties[i]].label;
    	}
    	else if (stringProperties.count(properties[i]) == 1){
    	    width = stringProperties[properties[i]].width;
    	    lbl = stringProperties[properties[i]].label;
    	}
        labels << std::setw(width) << lbl;
        labels << "|";
    }
    return(labels.str());
}

std::string Ticker::operator[](size_t index){
    std::ostringstream tickerLine;
    for (size_t i = 0; i < properties.size(); i++){
    	int width = 0;
    	if (numberProperties.count(properties[i]) == 1){
    	    width = numberProperties[properties[i]].width;
    	}
    	else if (stringProperties.count(properties[i]) == 1){
    	    width = stringProperties[properties[i]].width;
    	}
        tickerLine << std::setw(width) << stockList[stockList.symbols[index]][properties[i]];
        tickerLine << " ";
    }
    return(tickerLine.str());
}

void init(){
stockProperty prop;
prop.label = "Open";
prop.urlCode = "o0";
prop.width = 7;
numberProperties["OPEN"] = prop;

prop.label = "Close";
prop.urlCode = "p0";
prop.width = 7;
numberProperties["CLOSE"] = prop;

prop.label = "Price";
prop.urlCode = "l1";
prop.width = 7;
numberProperties["LAST TRADE PRICE"] = prop;

prop.label = "Ask";
prop.urlCode = "b2";
prop.width = 7;
numberProperties["ASK"] = prop;

prop.label = "Ask Size";
prop.urlCode = "a5";
prop.width = 8;
numberProperties["ASK SIZE"] = prop;

prop.label = "Avg Vol";
prop.urlCode = "a2";
prop.width = 9;
numberProperties["AVERAGE VOLUME"] = prop;

prop.label = "Volume";
prop.urlCode = "v0";
prop.width = 9;
numberProperties["VOLUME"] = prop;

prop.label = "Bid";
prop.urlCode = "b0";
prop.width = 7;
numberProperties["BID"] = prop;

prop.label = "Bid Size";
prop.urlCode = "b6";
prop.width = 8;
numberProperties["BID SIZE"] = prop;

prop.label = "BVPS";
prop.urlCode = "b4";
prop.width = 6;
numberProperties["BOOK VALUE PER SHARE"] = prop;

prop.label = "Change";
prop.urlCode = "c6";
prop.width = 6;
numberProperties["CHANGE"] = prop;

prop.label = "%Change";
prop.urlCode = "p2";
prop.width = 7;
numberProperties["%CHANGE"] = prop;

prop.label = "YR High";
prop.urlCode = "k0";
prop.width = 7;
numberProperties["YEAR HIGH"] = prop;

prop.label = "YR Low";
prop.urlCode = "j0";
prop.width = 7;
numberProperties["YEAR Low"] = prop;

prop.label = "Change Yr High";
prop.urlCode = "k4";
prop.width = prop.label.length();
numberProperties["CHANGE YEAR HIGH"] = prop;

prop.label = "Change Yr Low";
prop.urlCode = "j5";
prop.width = prop.label.length();
numberProperties["CHANGE YEAR LOW"] = prop;

prop.label = "%Change YR High";
prop.urlCode = "k5";
prop.width = prop.label.length();
numberProperties["%CHANGE YEAR HIGH"] = prop;

prop.label = "%Change YR Low";
prop.urlCode = "j6";
prop.width = prop.label.length();
numberProperties["%CHANGE YEAR LOW"] = prop;

prop.label = "Target";
prop.urlCode = "t8";
prop.width = 6;
numberProperties["YEAR TARGET PRICE"] = prop;

prop.label = "High";
prop.urlCode = "h0";
prop.width = 7;
numberProperties["DAY HIGH"] = prop;

prop.label = "Low";
prop.urlCode = "g0";
prop.width = 7;
numberProperties["DAY LOW"] = prop;

prop.label = "Divd";
prop.urlCode = "d0";
prop.width = 5;
numberProperties["DIVIDEND"] = prop;

prop.label = "%Divd";
prop.urlCode = "y0";
prop.width = 6;
numberProperties["%DIVIDEND"] = prop;

prop.label = "EPS(D)";
prop.urlCode = "e0";
prop.width = prop.label.length();
numberProperties["DILUTED EPS"] = prop;

prop.label = "EPS(CYR)";
prop.urlCode = "e7";
prop.width = prop.label.length();
numberProperties["EPS CURRENT YEAR"] = prop;

prop.label = "EPS(NQR)";
prop.urlCode = "e9";
prop.width = prop.label.length();
numberProperties["EPS NEXT QUARTER"] = prop;

prop.label = "EPS(NYR)";
prop.urlCode = "e8";
prop.width = prop.label.length();
numberProperties["EPS NEXT YEAR"] = prop;

prop.label = "PEG";
prop.urlCode = "r5";
prop.width = 5;
numberProperties["PEG"] = prop;

prop.label = "PE";
prop.urlCode = "r0";
prop.width = 7;
numberProperties["PE"] = prop;

prop.label = "PE RT";
prop.urlCode = "h0";
prop.width = 7;
numberProperties["PE RT"] = prop;

prop.label = "Book";
prop.urlCode = "p6";
prop.width = 7;
numberProperties["PRICE BOOK"] = prop;

prop.label = "EBITDA";
prop.urlCode = "j4";
prop.width = 7;
stringProperties["EBITDA"] = prop;

prop.label = "Divd Date";
prop.urlCode = "r1";
prop.width = prop.label.length();
stringProperties["DIVIDEND PAY DATE"] = prop;

prop.label = "MCap";
prop.urlCode = "j1";
prop.width = 7;
stringProperties["MARKET CAP"] = prop;

prop.label = "MCap RT";
prop.urlCode = "j3";
prop.width = 7;
stringProperties["MARKET CAP RT"] = prop;

prop.label = "Cur";
prop.urlCode = "c4";
prop.width = 5;
stringProperties["CURRENCY"] = prop;

prop.label = "Trade Time";
prop.urlCode = "t1";
prop.width = prop.label.length();
stringProperties["LAST TRADE TIME"] = prop;

prop.label = "Divd Date";
prop.urlCode = "q0";
prop.width = prop.label.length();
stringProperties["EX DIVIDEND DATE"] = prop;

prop.label = "Trade Date";
prop.urlCode = "d1";
prop.width = prop.label.length();
stringProperties["LAST TRADE DATE"] = prop;

prop.label = "Rev";
prop.urlCode = "s6";
prop.width = 7;
stringProperties["REVENUE"] = prop;

prop.label = "Exch";
prop.urlCode = "x0";
prop.width = 8;
stringProperties["EXCHANGE"] = prop;

prop.label = "Name";
prop.urlCode = "n0";
prop.width = 17;
stringProperties["NAME"] = prop;

prop.label = "Sym";
prop.urlCode = "s0";
prop.width = 5;
stringProperties["SYMBOL"] = prop;
}

bool cleanProperties(std::vector <std::string>& properties){
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
    	    format << numberProperties[properties[i]].urlCode;
    	}
    	else if (stringProperties.count(properties[i]) == 1){
    	    format << stringProperties[properties[i]].urlCode;
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
