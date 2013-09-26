#include <iostream>
#include <algorithm>
#include "yfinance.h"

int main(int argc, char* argv[]){
    yfinance::init();
    std::string layout = "open,close,last trade price,name,symbol,ask,average volume,volume,bid,book value per share,change,%change,year high,year low,change year high,change year low,%change year high,%change year low,year target price,day high,day low,dividend,%dividend,diluted eps,eps current year,eps next quarter,eps next year,peg,pe,pe rt,price book,ebitda,dividend pay date,market cap,market cap rt,currency,last trade time,ex dividend date,last trade date,revenue,exchange";
    std::string symbols = "bac,tsla,fish,FLWS,FCTY,FCCY,SRCE,FUBC,VNET,JOBS,EGHT,AVHI,SHLM,AAON,ASTM,ABAX,ABMD,AXAS,ACTG,rai,WLDN,WLFC,WIBC,WIN,WINA,WTFC,WTFCW,WETF,GULF,CHXF,EMCB,DGRE,GLCB,DXJS,DGRW,QQQX,NAFC,NATH,NAUH,NKSH,FIZZ,NCMI,NATI,NATL,NPBC,NRCIA,NRCIB,NSEC,NTSC,NWLI,NAII,ENG,ENPH,ESGR,ENTG,ETRM,EBTC,EFSC,EGT,ENMD,ENTR,ENVI,ENZN,EONC,EPIQ,EPZM,PLUS,EQIX,EAC,ERIC,ERIE,ESBF,ESCA,ESMC,ESPR,ESSA,ESSX,CLWT,EEFT,ESEA,EVEP,EVRY,EVOK,EVOL,EXA,EXAS,EXAC,EDS,ERW,EXEL,EXFO,EXLS,EXPE,EXPD,EXPO,ESRX,EXLP,MPB,MCEP,MBRG,MSEX,MOFG,MDXG,MNDO,MSPD,MRTX,MSON,MIND,MITK,MITL,MKSI,MINI,QCOR,QUIK,QDEL,QPACU,QNST,QUMU,DFZ,RRD,RADA,RDCM,ROIA,ROIAK,RSYS,RDNT,RDWR,RMBS,RAND,RLOG,GOLD,RPTP,RAVN,ROLL,RCMT,RDA,RLOC,RDI,RDIB,RSOL,RNWK,RP,RCPT,RCON,RECV,RRGB,RDHL,REDF,RGDO,REGN,RGLS,REIS,RELV,MARK,REMY,RNST,REGI,RCII,RTK,RENT,RGEN,RPRX,GRMN,GARS,GKNT,GENC,GNCMA,GFN,GFNCP,GENE,GNMK,GHDX,GNTX,THRM,GENT,GTIV,GNVC";
    yfinance::Ticker ticker(layout,symbols);
    std::cout << ticker.header() << std::endl;
    for (size_t i = 0; i < ticker.size(); i++){
        std::cout << ticker[i] << std::endl;
    }
    return(0);
}

