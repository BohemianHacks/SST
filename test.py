#!/usr/bin/python2.7
import cgi
import urllib2


form = cgi.FieldStorage()
data = ""

if form.has_key("s") and form.has_key("f"):
    if form["s"].value != "" and form["f"].value != "":
        symbols = form["s"].value
        format = form["f"].value
        url = ("http://download.finance.yahoo.com/d/quotes.csv?s="+symbols+"&f="+format)
        data = urllib2.urlopen(url).read()

print "Content-type: text/html\n"
print data
