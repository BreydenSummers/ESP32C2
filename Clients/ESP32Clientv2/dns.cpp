#include "dns.h"


void DNSQuery(const char* domain, IPAddress dnsServer, uint16_t dnsPort, WiFiUDP &udp) {
  

  // Construct a DNS query packet
  byte dnsQuery[512];
  memset(dnsQuery, 0, 512); // Clear the packet data

  // Transaction ID
  dnsQuery[0] = 0xAA; // Arbitrary ID
  dnsQuery[1] = 0xAA;

  // Flags: Standard query
  dnsQuery[2] = 0x01;
  dnsQuery[3] = 0x00;

  // Questions: 1
  dnsQuery[4] = 0x00;
  dnsQuery[5] = 0x01;

  // Answer RRs, Authority RRs, Additional RRs: 0

  // Query name (the domain, in DNS label format)
  int queryPos = 12; // Start of the Question Section
  const char* domainPart = domain;
  while (*domainPart) {
    const char* nextDot = strchr(domainPart, '.');
    size_t length = (nextDot) ? (nextDot - domainPart) : strlen(domainPart);
    dnsQuery[queryPos++] = length;
    memcpy(&dnsQuery[queryPos], domainPart, length);
    queryPos += length;
    if (!nextDot) break;
    domainPart = nextDot + 1;
  }
  dnsQuery[queryPos++] = 0; // End of the name

  // Type: A (host address)
  dnsQuery[queryPos++] = 0x00;
  dnsQuery[queryPos++] = 0x01;

  // Class: IN (Internet)
  dnsQuery[queryPos++] = 0x00;
  dnsQuery[queryPos++] = 0x01;

  // Send the DNS query over UDP to the DNS server.
  udp.beginPacket(dnsServer, dnsPort);
  udp.write(dnsQuery, queryPos);
  udp.endPacket();
  Serial.println("DNS query sent.");






}


IPS parseDNSResponse(byte* response, int responseSize) {
  IPS ips;
  // Check for a minimum length (header + some data)
  if (responseSize < (12 + 5)) {
    Serial.println("Response too short to be valid");
    return ips;
  }

  // Transaction ID check skipped for simplicity

  // Check response flags (QR, OpCode, AA, TC, RD, RA, Z, RCODE)
  if ((response[2] & 0x80) == 0) {
    Serial.println("Not a response packet");
    return ips;
  }
  
  // Check for errors in the response (RCODE)
  if (response[3] & 0x0F) {
    Serial.println("DNS error in response");
    return ips;
  }

  // Get answer count
  int answerCount = (response[6] << 8) | response[7];
  Serial.print("Answers: ");
  Serial.println(answerCount);

  if (answerCount == 0) {
    Serial.println("No answers in response");
    return ips;
  }

  // Skip the question section
  int index = 12; // Start of question section
  while (response[index] != 0) { // Skip the QNAME
    index++;
    if (index >= responseSize) return ips; // Safety check
  }
  index += 5; // Skip the zero byte, QTYPE, and QCLASS

  // Process answer section
  const int ten = 10;
  const int four = 4;

  for (int answer = 0; answer < answerCount; answer++) {
    // Skip the NAME field (which is a pointer in responses)
    index += 2; // Normally, you'd check for pointers/compression here

    // Check for end of packet
    if (index >= responseSize - 10) return ips; // Need at least 10 more bytes

    // Check TYPE (we're looking for A records, TYPE 1)
    if (response[index] == 0 && response[index + 1] == 1) {
      index += 8; // Skip TYPE, CLASS, and TTL
      int dataLength = (response[index] << 8) | response[index + 1];
      index += 2;
      if (dataLength == 4) { // IPv4 address should be 4 bytes long
        Serial.print("IP Address: ");
        ips.arr[answer] = IPAddress(response[index], response[index + 1], response[index + 2], response[index + 3]);
        for (int i = 0; i < 4; i++) {
          Serial.print(response[index + i], DEC);
          if (i < 3) Serial.print(".");
        }
        Serial.println();
      }
      index += dataLength; // Skip to next record
    } else {
      // For simplicity, this code does not handle other record types like CNAME or MX
      index += 10; // Skip TYPE, CLASS, TTL
      int dataLength = (response[index] << 8) | response[index + 1];
      index += 2 + dataLength; // Skip to next record
    }
  }
  ips.init = true;
  return ips;
}