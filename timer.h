#include <ctime>
#include <cstdint>
#include <cstring>
#include <string>

//Basic timer object that simplifies time tracking and stamping

class Timer{
    private:
        time_t rawtime; //Time since epoch
        struct tm * ptm; //Time struct pointer
    public:
        Timer();
        uint_fast8_t hr(); //Hour in 24 hour format
        uint_fast8_t min();
        uint_fast8_t sec();
        uint_fast8_t day(); //Day of month
        uint_fast8_t mon(); //Month 1-12
        uint_fast8_t yr(); //Actual year for date
        std::string stamp; //Time Stamp string from last time timeStamp() was called
        std::string& timeStamp();
};

Timer::Timer(){
        time(&rawtime);
        ptm = localtime(&rawtime);
        char ts[20];
        sprintf(ts, "%02d-%02d-%4d %2d:%02d:%02d", ptm->tm_mon+1, ptm->tm_mday, ptm->tm_year+1900, (ptm->tm_hour)%24, ptm->tm_min, ptm->tm_sec);
        stamp = ts;
}

uint_fast8_t Timer::hr(){
    time(&rawtime);
    ptm = localtime(&rawtime);
    return(ptm->tm_hour);
}

uint_fast8_t Timer::min(){
    time(&rawtime);
    ptm = localtime(&rawtime);
    return(ptm->tm_min);
}

uint_fast8_t Timer::sec(){
    time(&rawtime);
    ptm = localtime(&rawtime);
    return(ptm->tm_sec);
}

uint_fast8_t Timer::day(){
    time(&rawtime);
    ptm = localtime(&rawtime);
    return(ptm->tm_mday);
}

uint_fast8_t Timer::mon(){
    time(&rawtime);
    ptm = localtime(&rawtime);
    return(ptm->tm_mon+1);
}

uint_fast8_t Timer::yr(){
    time(&rawtime);
    ptm = localtime(&rawtime);
    return(ptm->tm_year+1900);
}

std::string& Timer::timeStamp(){
    time(&rawtime);
    ptm = localtime(&rawtime);
    char ts[20];
    sprintf(ts, "%02d-%02d-%4d %2d:%02d:%02d", ptm->tm_mon+1, ptm->tm_mday, ptm->tm_year+1900, (ptm->tm_hour)%24, ptm->tm_min, ptm->tm_sec);
    stamp = ts;
    return stamp;
}
