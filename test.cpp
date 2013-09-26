#include <iostream>
#include <algorithm>
#include "yfinance.h"

int main(int argc, char* argv[]){
    yfinance::init();
    std::string layout = "open,close,last trade price,name,symbol,ask,average volume,volume,bid,book value per share,change,%change,year high,year low,change year high,change year low,%change year high,%change year low,year target price,day high,day low,dividend,%dividend,diluted eps,eps current year,eps next quarter,eps next year,peg,pe,pe rt,price book,ebitda,dividend pay date,market cap,market cap rt,currency,last trade time,ex dividend date,last trade date,revenue,exchange";
    std::string symbols = "bac,tsla,fish,FLWS,FCTY,FCCY,SRCE,FUBC,VNET,JOBS,EGHT,AVHI,SHLM,AAON,ASTM,ABAX,ABMD,AXAS,ACTG,rai,WLDN,WLFC,WIBC,WIN,WINA,WTFC,WTFCW,WETF,GULF,CHXF,EMCB,DGRE,GLCB,DXJS,DGRW,QQQX,NAFC,NATH,NAUH,NKSH,FIZZ,NCMI,NATI,NATL,NPBC,NRCIA,NRCIB,NSEC,NTSC,NWLI,NAII,ENG,ENPH,ESGR,ENTG,ETRM,EBTC,EFSC,EGT,ENMD,ENTR,ENVI,ENZN,EONC,EPIQ,EPZM,PLUS,EQIX,EAC,ERIC,ERIE,ESBF,ESCA,ESMC,ESPR,ESSA,ESSX,CLWT,EEFT,ESEA,EVEP,EVRY,EVOK,EVOL,EXA,EXAS,EXAC,EDS,ERW,EXEL,EXFO,EXLS,EXPE,EXPD,EXPO,ESRX,EXLP,EXTR,EZCH,EZPW,FFIV,FB,FRP,FWM,FALC,DAVE,FARM,FFKT,FMNB,FARO,FAST,FATE,FBSS,FBRC,FDML,FNHC,FFCO,FEIC,FHCO,ONEQ,LION,FDUS,FRGI,FSC,FSCFL,FSFR,FITB,FNGN,FISI,FNSR,FEYE,FBNC,FNLC,BUSE,FBIZ,FCVA,FCAP,FCFS,FCZA,FCNCA,FCLF,FCBC,FCCO,FBNK,FDEF,FFBH,FFNM,FFBC,FFBCW,FFIN,THFF,SCBT,FFNW,FFKY,INBK,FIBK,FRME,FMBI,NBCB,FNFG,FSFG,FSGI,FSLR,FSBK,FTCS,BICK,CU,PLTM,CARZ,FTSL,HYLS,TDIV,YDIV,MDIV,SKYY,QABA,FONE,GRID,QCLN,QQEW,QQXT,QTEC,FUNC,FBMI,SVVC,FMER,FSRV,FISV,FIVE,FPRX,FLML,FLXS,FLEX,FLIR,FLOW,FLDM,FFIC,FONR,VIFL,FES,FORM,FORTY,FORR,FTNT,FWRD,FORD,FOSL,FWLT,FMI,FXCB,FOXF,FRAN,FELE,FRNK,FRED,FREE,RAIL,FEIM,FTR";
    yfinance::Ticker ticker(layout,symbols);
    std::cout << ticker.header() << std::endl;
    for (size_t i = 0; i < ticker.size(); i++){
        std::cout << ticker[i] << std::endl;
    }
    return(0);
}

