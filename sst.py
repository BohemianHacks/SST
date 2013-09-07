import urllib2
import os
import time
from string import upper

#symbols to follow
symbols = ['rai','amgn','goog','tsla']
#interval in seconds to update
interval = 15
#container for stock objects
stocks = []
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
        sTime = time.time()
        for s in stocks:
		s.refresh()
        os.system('clear')
	print('Last Updated: '+time.strftime("%H:%M:%S", time.gmtime()))
        for s in stocks:
        	percent = round(100*s.change/s.current,2)
                if s.change < 0:
                	print('{1:4}:{2}{0:7.2f} '.format(s.current,upper(s.symbol),red)+str(round(s.change,2)).zfill(6)+' -%'+str(abs(percent))+endc)
                else:
                        print('{1:4}:{2}{0:7.2f} '.format(s.current,upper(s.symbol),green)+'+'+str(round(s.change,2)).zfill(5)+' +%'+str(abs(percent))+endc)
	eTime = time.time()
	time.sleep(float(interval)-float(eTime-sTime))
