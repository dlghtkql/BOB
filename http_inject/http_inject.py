from scapy.all import *

'''
    https://github.com/invernizzi/scapy-http
    http://stackoverflow.com/questions/18705953/scapy-raw-data-manipulation
    http://stackoverflow.com/questions/4750793/python-scapy-or-the-like-how-can-i-create-an-http-get-request-at-the-packet-leve
'''

pkt = Raw()

def make_forward_packet(pkt):
    fpacket = pkt.copy()
    fpacket[TCP].flags = 'FA'
    fpacket[TCP].load = 'blocked\r\n'
    fpacket[TCP].seq = pkt[TCP].seq + len(pkt.getlayer(Raw))
    del fpacket[IP].len 

    return fpacket

def make_backword_packet(pkt):
    #print pkt.show()
    bpacket = pkt.copy()
    var_packet = pkt.copy()
    
    bpacket[TCP].flags = 'FA'
    bpacket[TCP].load = 'blocked\r\n'
    del bpacket[IP].len
    
    bpacket[TCP].ack = pkt[TCP].seq + len(pkt.getlayer(Raw))

    bpacket[Ether].src = var_packet[Ether].dst
    bpacket[Ether].dst = var_packet[Ether].src

    bpacket[IP].src = var_packet[IP].dst
    bpacket[IP].dst = var_packet[IP].src

    bpacket[TCP].dport = var_packet[TCP].sport
    bpacket[TCP].sport = var_packet[TCP].dport
    #bpacket = Ether(src=var_packet[Ether].dst,dst=var_packet[Ether].src)/IP(src=var_packet[IP].dst,dst=var_packet[IP].src)/TCP(dport=var_packet[TCP].sport,sport=var_packet[TCP].dport,flags='FA',ack=ack_num)/'blocked\r\n'

    return bpacket

def make_redirect_packet(pkt):
    
    rpacket = pkt.copy()
    var_packet = pkt.copy()

    redirect_site = 'HTTP/1.1 302 Redirect\r\n'
    redirect_site += 'Location: http://www.gilgil.net\r\n'
    redirect_site += '\r\n'

    rpacket[TCP].load = redirect_site
    rpacket[TCP].flags = 0
    rpacket[TCP].ack = pkt[TCP].seq + len(pkt.getlayer(Raw))
    del rpacket[IP].len

    rpacket[Ether].src = var_packet[Ether].dst
    rpacket[Ether].dst = var_packet[Ether].src

    rpacket[IP].src = var_packet[IP].dst
    rpacket[IP].dst = var_packet[IP].src

    rpacket[TCP].dport = var_packet[TCP].sport
    rpacket[TCP].sport = var_packet[TCP].dport

    return rpacket

def http_packet(packet):
    if packet[TCP].dport == 80:   
        
        fpkt = make_forward_packet(packet)
        sendp(fpkt)
        
        bpkt = make_backword_packet(packet)
        sendp(bpkt)

        rpkt_http = make_redirect_packet(packet)
        sendp(rpkt_http)
    
sniff(filter='tcp port 80',lfilter=lambda p: "GET" in str(p),prn=http_packet,count=1)   #http packet capture


