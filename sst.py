#!/usr/bin/python2.7
import urllib2
import cgi
import StringIO
import csv
import json


def getData(symbols):
    url = ("http://download.finance.yahoo.com/d/quotes.csv?s="+symbols+"&f=n0l1p2op")
    data = urllib2.urlopen(url).read()
    return data

def getJson(csvData):
    f = StringIO.StringIO(csvData)
    reader = csv.DictReader( f, fieldnames = ( "name","last","change","open","close" ))
    out = json.dumps( [ row for row in reader ] ) 
    return out

def cgiCall(form):
    data = ""
    if form.has_key("s"):
        if form["s"].value != "":
            symbols = form["s"].value
            data = getJson(getData(symbols))
    return data

form = cgi.FieldStorage()
print "Content-type: text/html\n"
print cgiCall(form)

    
