#include <iostream>
#include <algorithm>
#include "yfinance.h"

int main(int argc, char* argv[]){
    yfinance::init();
    std::string layout = "open,close,last trade price,name,symbol,ask,average volume,volume,bid,book value per share,change,%change,year high,year low,change year high,change year low,%change year high,%change year low,year target price,day high,day low,dividend,%dividend,diluted eps,eps current year,eps next quarter,eps next year,peg,pe,pe rt,price book,ebitda,dividend pay date,market cap,market cap rt,currency,last trade time,ex dividend date,last trade date,revenue,exchange";
    std::string symbols = "AEGR,AEGN,AEHR,AEPI,AVAV,AEZS,ATRM,AFCE,AFFX,AGEN,AGYS,AGIO,AIRM,AIRT,ATSG,AMCN,AIXG,AKAM,AKRX,ALSK,AMRI,ALCS,ADHD,ALXN,ALXA,ALCO,ALGN,ALIM,ALKS,ALGT,ALLB,AFOP,AIQ,AHGP,ARLP,AHPI,AMOT,ALLT,MDRX,AFAM,ALNY,AOSL,ATEC,ASDR,EMDR,RUDR,ALTI,ALTR,ASPS,APSA,AIMC,ALVR,AMAG,AMRN,AMZN,AMBC,AMBCW,AMBA,EPAX,AMBT,AMBI,AMCX,AMED,UHAL,ASBI,ATAX,AMOV,AGNC,AGNCP,MTGE,ACAS,ANCI,AETI,AMIC,AMNB,ANAT,APFC,APEI,ARII,ARCP,AMRB,ASEI,AMSWA,AMSC,AMWD,CRMT,ABCB,AMSF,ASRV,ASRVP,ATLO,AMGN,FOLD,AMKR,AMSG,ASYS,AFSI,AMRS,ANAC,ANAD,ADI,ALOG,ANLY,ANEN,ANCB,AMCF,ANDA,ANDAU,ANDAW,ANGI,ANGO,ANIP,ANIK,ANSS,ATRS,ANTH,APAGF,ATNY,APOG,APOL,AINV,AAPL,ARCI,AMAT,AMCC,AREX,APRI,AQU,AQUUU,AQUUW,PETX,ARCW,ABIO,ACGL,ACAT,ARDNA,ARNA,ARCC,AGII,AGIIL,ARIA,ARKR,ABFS,ARMH,ARTX,ARQL,ARRY,ARRS,AROW,ARWR,ARTNA,ARTC,ARTW,ARUN,ASBB,ASNA,ASCMA,ASTI,AERL,APWC,ASIA,ASMI,ASML,AZPN,ASBC,ASBCW,ASFI,ATEA,ASTE,ASTX,ALOT,ATRO,ASTC,ASUR,ATAI,ATHN,AFCB,ATHX,AAME,ACFC,ATNI,ATLC,AAWW,AFH,ATML,ATMI,ATOS,ATRC,ATRI,ATTU,AUBN,ADNC,AUDC,ADAT,ABTL,ADSK,ADP,AMAP,AUXL,AVGO,AVNR,AVEO,AVNW,AVID,CAR,AWRE,ACLS,AXGN,AXTI";
    yfinance::Ticker ticker(layout,symbols);
    std::cout << ticker.header() << std::endl;
    for (size_t i = 0; i < ticker.size(); i++){
        std::cout << ticker[i] << std::endl;
    }
    return(0);
}

