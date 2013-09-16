#include <cstdint>
#include <string>

class Stock{
    private:
        int_fast64_t open; //Today's opening price
        int_fast64_t close; //Yesterdays closing price
        int_fast64_t current; //Current price
        int_fast64_t change; //Percent change between current and close
        std::string symbol; //Ticker symbol
    public:
        void update();
        Stock(const std::string sym);
        int_fast64_t getOpen(){return open;};
        int_fast64_t getClose(){return close;};
        int_fast64_t getCurrent(){return current;};
        int_fast64_t getChange(){return change;};
        std::string getSymbol(){return symbol;};
};
