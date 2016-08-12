#!/usr/bin/python
#sudo python mal_site.py
#2016.8.4

import socket
from scapy.all import *
import os
import subprocess
import shlex
import fcntl
import struct
import string
import thread
from time import sleep
from scapy.layers import http
import codecs
import re
import datetime

############# read mal_site.txt file ########
mal_list = []               # store for mal_site
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

f1 = open("mal_log",'a+') 

#======================= Get My IP,MAC and Gateway IP section ==========================
def Mac_address(ifname):
	s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	info = fcntl.ioctl(s.fileno(),0x8927,struct.pack('256s',ifname[:15]))
	myMac = ''.join(['%02x:' % ord(char) for char in info[18:24]])[:-1]
	return myMac

def Ip_address(ifname):
	s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	info = fcntl.ioctl(s.fileno(),0x8927,struct.pack('256s',ifname[:15]))
	myIp = socket.inet_ntoa(fcntl.ioctl(s.fileno(),0x8915,struct.pack('256s',ifname[:15]))[20:24])
	return myIp

strs = subprocess.check_output(shlex.split('ip r l'))
gateway = strs.split('default via')[-1].split()[0].split('/')[0]		# gateway ip(gateway)

My_Macaddr = Mac_address('enp0s5')			# my Mac_address (My_Macaddr)
My_ipaddr = Ip_address('enp0s5')			# my Ip_address (My_ipaddr)

#======================== Send broadcast packet section ==================
p_dst = raw_input("input destination ip:")

second_packet = srp(Ether(dst="ff:ff:ff:ff:ff:ff")/ARP(op=1,pdst=gateway))
gateway_mac = second_packet[0][0][1][Ether].src          # gateway mac (gateway_mac)

first_packet = srp(Ether(dst="ff:ff:ff:ff:ff:ff")/ARP(op=1, pdst=p_dst ))
source = first_packet[0][0][1][Ether].src		# sender source mac (source)
psource = first_packet[0][0][1][Ether].psrc         # sender ip (psource)

print "=====MAC, IP ,gateway====="
print "My MAC:"+My_Macaddr
print "My IP:"+My_ipaddr
print "Gateway IP:"+gateway
print "Gateway Mac:"+gateway_mac
print "Sender Mac:"+source
print "Sender IP:"+psource
print "=========================="
#============================= Relay packet section ========================
def send_packet(packet):
    
    if packet.haslayer(http.HTTPRequest):   
        host_name = packet[0][0].Host
        host_name = host_name.replace("\r\n","")    # remove '\r\n'
        host_name = host_name.replace("www.","")    # remove ''www.

        for mal in mal_list:
            if mal == host_name:               # compare mal_list and url(host_name)
                print mal
                s = str(datetime.datetime.now())
                f1.writelines(s+"/ ip_address: "+packet[0][1].src+"/ host: "+host_name+"\n")
                continue		# if mal_site then continue
            else:
                packet[0][0].src = My_Macaddr
                packet[0][0].dst = gateway_mac
                sendp(packet)        
   
    else:
        packet[0][0].src = My_Macaddr
        packet[0][0].dst = gateway_mac
        sendp(packet)

sniff(filter='ip',prn=send_packet)
#sniff(filter='tcp port 80',lfilter = lambda p:'GET' in str(p) ,prn=send_packet)  
f1.close()
