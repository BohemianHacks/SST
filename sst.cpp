#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <curl/curl.h>

using namespace std;

//callback I found online for curl that sends a string of webdata
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	    ((string*)userp)->append((char*)contents, size * nmemb);
	        return size * nmemb;
}

//scrape basic html or text based on curl example code
bool getPage(const char* url, string& readBuffer){
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

//gets csv data from yahoo finance and returns as a double
double getPrice(const string& symbol, const string type){
	stringstream urlBuilder;
	string response;
	double price;
	urlBuilder << "http://download.finance.yahoo.com/d/quotes.csv?s=" << symbol << "&f=" << type;
	for (uint_fast8_t i = 0; i < 3; i++){
		if (getPage(urlBuilder.str().c_str(),response)){
			price = atof(response.c_str());
		}
		if (price > 0.0){
			return price;
		}
	}
	cout << "Could not fetch price: " << symbol << endl;
	return price;
}

//start of stock class to hold all the data and compute technicals
class Stock {
	private:
		double open; //opening price
		double close; //previous closing price
		double current; //current price
		double change; //percent change from previous close
		string symbol; //stock ticker symbol
	public:
		double getOpen(){return open;};
		double getClose(){return close;};
		double getCurrent(){return current;};
		double getChange(){return change;};
		const string getSymbol(){return symbol;};
		Stock(const string& sym);
		void update(void);
		void getEma(int days);
};

Stock::Stock(const string& sym){
	symbol = sym;
	open = getPrice(symbol,"o");
	close = getPrice(symbol,"p");
	current = getPrice(symbol,"l1");
	change = 100.0*(current-close)/current;
}


int main(int argc, char *argv[]){
	// color  and updated inteveral is yet to be implemented
	bool color = false;
	int interval = 30;
	vector<string> stocks;
	if (argc<2){
		cout << "Usage: sst [options] <stock list>\n";
		return 0;
	}
	else if (argc>=2){
		for (size_t i=1; i<argc; i++){
			//look for flags or filename to load
			if (strstr(argv[i],"-") != NULL){
				if (strcmp(argv[i],"-c")==0){
					color = true;
					}
			}
			else{
				//lack of tack assumed to be filename
				ifstream list;
				list.open(argv[i]);
				if (list.is_open()){
					//some variables to load the stock symbol vector
					int j = 0;
					string sym;
					while (list >> sym){
						stocks.push_back(sym);
					}
				}
				else{
					cout << "Usage: sst [options] <stock list>\n";
					return 0;
				}
			}
		}
		if (!(0 < stocks[0].length() < 6)){
			cout << "Usage: sst [options] <stock list>\n";
			return 0;
		}
	}
	//vector to hold stock objects
	vector<Stock> stockV;
	for (size_t i = 0; i < stocks.size(); i++){
		double price = getPrice(stocks[i], "l1");
		//check for a valid price before loading stock
		if (price > 0.0){
			Stock s(stocks[i]);
			stockV.push_back(s);
		}
		//stop looking for stocks after a blank line
		else if (stocks[i].length() < 1)
			break;
	}
	//I would like to loop here and update after interval seconds.
	//a good cross platform for unbuffered keyboard input is hard to find.
	cout <<  "Symbol Current  %Change\n";
	for (size_t s = 0;s < stockV.size();s++){
		cout << setw(5) <<  stockV[s].getSymbol() << ":";
		printf("%7.2f %8.2f%% \n", stockV[s].getCurrent(), stockV[s].getChange());
	}
		cin.ignore();	
	return 0;
}
