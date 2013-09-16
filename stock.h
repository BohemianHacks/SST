#include <cstdint>
#include <string>

class Stock{
    protected:
        int_fast32_t open; //Today's opening price
        int_fast32_t close; //Yesterdays closing price
        int_fast32_t current; //Current price
        int_fast32_t change; //Percent change between current and close
        std::string symbol; //Ticker symbol
        std::string name;
    public:
        friend Stock getData();
        void update();
        Stock(const std::string& sym):symbol(sym){};
};
