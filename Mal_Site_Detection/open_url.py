#sudo python 

import webbrowser
import codecs
import time

mal_list = []               # store for mal_site

f = codecs.open('mal_site.txt','r','utf-8')
line = f.readline()

while line:
    mal_list.append(line),
    line = f.readline()
f.close()

i = 0
for file1 in mal_list:
#while i<50:
    url = mal_list[i]
    webbrowser.open(url)
    time.sleep(20)
    i = i+1
