import urllib2
import os
from time import gmtime
from string import upper

#symbols to follow
symbols = ['rai','amgn','goog','tsla']
#interval in minutes to update
interval = 1

stocks = []
lTime = 0

#currently using ansi to add minor coloring to output
green = '\033[92m'
red = '\033[91m'
endc = '\033[0m'

#basic stock object to contain pricing info
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

#populate stock list
for s in symbols:
        stocks.append(stock(s))

#run until terminated isn't very clean but works for my minor background needs
while True:
        cTime = gmtime().tm_min
        if cTime-lTime >= interval:
                #Every interval minutes, refresh the stocks and print the changes on a clean screen
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
