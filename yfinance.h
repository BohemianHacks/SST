#include <curl/curl.h>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <vector>
#define baseURL "http://download.finance.yahoo.com/d/quotes.csv?s="

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

bool getPrice(const std::string& symbol, int_fast64_t& price){
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

bool getData(std::vector <std::string> symbols, std::string format){
    std::stringstream urlBuilder;
    std::string response;
    std::stringstream syms;
    urlBuilder << baseURL << syms.str() << "&f=" << format;
    for (uint_fast8_t i = 0; i < 3; i++){
        if (getPage(urlBuilder.str().c_str(),response)){
            break;
        }
    }
    return true;
}
