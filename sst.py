import urllib2
import os
from time import gmtime
from string import upper

symbols = ['rai','amgn','goog','tsla']
stocks = []
lTime = 0
interval = 1
green = '\033[92m'
red = '\033[91m'
endc = '\033[0m'

class stock:
        def __init__(self,symbol):
                self.symbol = symbol
                self.url = 'http://download.finance.yahoo.com/d/quotes.csv?s='+symbol
                self.current = float(urllib2.urlopen(self.url+'&f=l1').read())
                self.open = float(urllib2.urlopen(self.url+'&f=p').read())
                self.change = 0.0

        def refresh(self):
                self.current = float(urllib2.urlopen(self.url+'&f=l1').read())
                self.open = float(urllib2.urlopen(self.url+'&f=p').read())
                self.change = self.current-self.open

for s in symbols:
        stocks.append(stock(s))

while True:
        cTime = gmtime().tm_min
        if cTime-lTime >= interval:
                for s in stocks:
                        s.refresh()
                lTime=cTime
                os.system('clear')
                print('Last Updated: '+str(gmtime().tm_hour)+':'+str(gmtime().tm_min))
                for s in stocks:
                        percent = round(100*s.change/s.current,2)
                        if s.change < 0:
                                print(upper(s.symbol)+":"+red+str(s.current)+' '+str(round(s.change,2))+' -%'+str(abs(percent))+endc)
                        else:
                                print(upper(s.symbol)+":"+green+str(s.current)+' +'+str(round(s.change,2))+' +%'+str(percent)+endc)
