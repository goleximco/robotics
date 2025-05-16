#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>


IPAddress my_ip(192, 168, 0, 177);
IPAddress ip_client(192, 168, 0, 103);
byte my_mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
unsigned int localPort = 8080;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
char  ReplyBuffer[] = "";


EthernetUDP Udp;
void udp_setup() 
{
  Ethernet.begin(my_mac, my_ip);
  Udp.begin(localPort);
}



void udp_check() 
{
  int packetSize = Udp.parsePacket();
  if (packetSize) 
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i = 0; i < 4; i++) 
    {
      Serial.print(remote[i], DEC);
      if (i < 3) 
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    memset(packetBuffer, 0, sizeof(packetBuffer));
  }
  delay(10);
}

void udp_send(String u)
{
    Udp.beginPacket(ip_client, localPort);
    Udp.print(u);
    Udp.endPacket();   
}
