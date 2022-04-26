#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "************";  // Enter SSID here
const char* password = "************";  //Enter Password here

//const char* ssid = "UCB Wireless";  // Enter SSID here
//const char* password = "";  //Enter Password here

WiFiUDP Udp;
unsigned int localUdpPort = 1999;  // local port to listen on
char packetBuffer[255]; //buffer to hold incoming packet

void setup() 
{
  Serial.begin(115200);
  delay(100);

  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  // connect to your local wi-fi network
  WiFi.begin(ssid, password);
  
  // check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  Udp.begin(localUdpPort);
   
   // initialize digital pin 12 as an output.
   pinMode(12, OUTPUT);
}


void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize) 
  {
    Serial.print("From: ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);

    int len = Udp.read(packetBuffer, 255);
    if (len > 0)
    {
      packetBuffer[len] = 0;
    }
    
    Serial.print(", Contents: ");
    Serial.println(packetBuffer);
  }

  if (packetBuffer[0] == 'y')
    digitalWrite(12, HIGH);   // atomize
  else
    digitalWrite(12, LOW);

//  digitalWrite(12, HIGH); // for debugging
}
