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
                self.previous = float(urllib2.urlopen(self.url+'&f=p').read())
                self.change = 0.0

        def refresh(self):
                self.current = float(urllib2.urlopen(self.url+'&f=l1').read())
                self.previous = float(urllib2.urlopen(self.url+'&f=p').read())
                self.change = self.current-self.previous

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
                print('Last Updated: '+str(gmtime().tm_hour)+':'+str(gmtime().tm_min).zfill(2))
                for s in stocks:
                        percent = round(100*s.change/s.current,2)
                        if s.change < 0:
                                print('{1:4}:{2}{0:7.2f} '.format(s.current,upper(s.symbol),red)+str(round(s.change,2)).zfill(6)+' -%'+str(abs(percent)).zfill(5)+endc)
                        else:
                                print('{1:4}:{2}{0:7.2f} '.format(s.current,upper(s.symbol),green)+'+'+str(round(s.change,2)).zfill(5)+' +%'+str(abs(percent)).zfill(5)+endc)
