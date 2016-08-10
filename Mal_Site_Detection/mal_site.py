#!/usr/bin/python
#sudo python mal_site.py

import codecs
import re
import datetime
from scapy.all import *
from scapy.layers import http

mal_list = []               # store for mal_site

############# read mal_site.txt file ########
f = codecs.open('mal_site.txt','r','utf-8')
line = f.readline()

while line:
    mal_list.append(line),
    line = f.readline()
f.close()


i = 0
j = 0
k = 0
q = 0
############## arrange list ###################
for line in mal_list:
    mal_list[i] = line.replace("http://","")        # remove 'http://'
    i = i+1
for line in mal_list:
    mal_list[j] = line.replace("www.","")           # remove 'www.'
    j = j+1
for line in mal_list:
    mal_list[k] = line.replace("\n","")             # remove '\n'
    k = k+1
for line in mal_list:
    if line[-1] == '/':
        mal_list[q] = line.replace(line[-1],"")     # remove last slash
    q = q+1

f1 = open('mal_log','a+')
############# sniff http packet and URL ########
def http_packet(packet):
    if http.HTTPRequest or http.HTTPResponse in packet:
        host_name = packet[0][0].Host
        host_name = host_name.replace("\r\n","")    # remove '\r\n'
        host_name = host_name.replace("www.","")    # remove ''www.
        print "------"+host_name                # check all site packet

        for mal in mal_list:
            if mal == host_name:               # compare mal_list and url(host_name)
                print mal
                s = str(datetime.datetime.now())
                f1.writelines(s+"/ ip_address: "+packet[0][1].src+"/ host: "+host_name+"\n")

packet = sniff(filter='tcp port 80',lfilter = lambda p:'Host' in str(p),prn=http_packet)    # if program not work, please change 'Host' -> 'GET'
f1.close()

