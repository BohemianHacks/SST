#include <cstdint>
#include <cmath>
#include <iostream>

class Price{
    private:
        int_fast16_t dollars;
        uint_fast8_t cents;
    public:
        Price(double& price);
        double getFloat(){return (double(dollars)+(double(cents)/100.0));};
        Price& operator=(const Price& rhs);
        Price& operator+=(const Price& rhs);
        Price& operator-=(const Price& rhs);
};

Price::Price(double& price){
    dollars = (int)floor(price);
    cents = (int)(price*100.0)%100;
}

Price& Price::operator=(const Price& rhs){
        if(&rhs == this) {return *this;}
        dollars = rhs.dollars;
        cents = rhs.cents;
        return *this;
}

Price& Price::operator+=(const Price& rhs){
        dollars += rhs.dollars + (int_fast16_t)(rhs.cents + cents)/100;
        cents = (rhs.cents + cents)%100;
        return *this;
}

Price& Price::operator-=(const Price& rhs){
        if (cents < rhs.cents){
            dollars -= rhs.dollars + 1;
            cents = (cents + 100) - rhs.cents;
        }
        else{
            dollars -= rhs.dollars;
            cents -= rhs.cents;
        }
}

