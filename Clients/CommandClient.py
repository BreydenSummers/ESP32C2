#!/usr/bin/env python3

import dns.query
import dns.resolver
import dns.message




def makeQuery(command, ip, domain):
    record_type = 'A'  # Can be 'MX', 'NS', 'TXT', etc.
    dns_server = '67.177.34.64'  # Example DNS server
    dns_port = 53533  # Example non-standard port
    # Prepare the query
    query = dns.message.make_query(domain, record_type) 

    try:
        # Perform the DNS query to the specified server and port
        response = dns.query.udp(query, dns_server, port=dns_port)
        # Parsing the response
        if response.answer:
            for answer in response.answer:
                print(f'Response from {dns_server}:{dns_port} for {domain} ({record_type}): {answer.to_text()}')
        else:
            print(f'No {record_type} records found for {domain} on {dns_server}:{dns_port}.')
    except Exception as e:
        print(f'Error querying {dns_server}:{dns_port} for {domain}: {e}')


while True:
    # Command line interface for asking the user to enter a command and then an ip address
    command = input("Enter a command: ")
    ip = input("Enter an IP address: ")
    domain = f'command.{command}.{ip.replace(".", "-")}.com'
    makeQuery(command, ip, domain)
