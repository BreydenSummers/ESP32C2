#ifndef dns_h
#define dns_h
#include <Arduino.h>
#include <WiFiUdp.h>
#include <WiFi.h>

#define IPARRAYSIZE 10

struct IPS {
    IPAddress arr[IPARRAYSIZE];
    bool init = false;
};

void DNSQuery(const char* domain, IPAddress dnsServer, uint16_t dnsPort, WiFiUDP &udp);
IPS parseDNSResponse(byte* response, int responseSize);

#endif
