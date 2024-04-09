import socket
from dnslib import DNSRecord, QTYPE, RR, A

def dns_response(data):
    request = DNSRecord.parse(data)
    
    print("Received query: ", request)
    
    reply = request.reply()
    qname = request.q.qname
    qtype = request.q.qtype
    ips = ["1.5.5.5","2.6.6.6", "3.5.5.5"]  # Example IP address to return for all queries
    
    # Create a DNS reply record
    for ip in ips:
        reply.add_answer(RR(qname, QTYPE.A, rdata=A(ip), ttl=60))
    
    # Return the reply packet
    return reply.pack()

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.bind(("0.0.0.0", 53533))
        print("DNS Server listening on port 53")
        
        while True:
            data, addr = s.recvfrom(512)
            response = dns_response(data)
            s.sendto(response, addr)

if __name__ == "__main__":
    main()

