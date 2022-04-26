#include <WiFi.h>
#include <WiFiUdp.h>
#include "heltec.h"

const char* ssid = "***********";  // Enter SSID here
const char* password = "***********";  //Enter Password here

WiFiUDP Udp;
unsigned int localUdpPort = 1999;  // local port to listen on
IPAddress IP_Remote(192, 168, 0, 255);   // Broadcast IP Address

const int GSR=36;
int sensorValue=0;
int gsr_average=0;
bool activateScent = false;
 
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

  Heltec.begin(true, false, true);
  Heltec.display->setContrast(255);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
}
 
void loop()
{  
  long sum=0;
  for(int i=0;i<10;i++)           //Average the 10 measurements to remove noise
  {
      analogReadResolution(10);
      sensorValue=analogRead(GSR);
      sum += sensorValue;
      delay(5);
   }
   
   gsr_average = sum/10;
   
   if (gsr_average > 100)
    activateScent = true;
   else
    activateScent = false;


  Heltec.display->clear();
  Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->drawString(64, 16, String(gsr_average));
  Heltec.display->setFont(ArialMT_Plain_16);
  if (activateScent)
    Heltec.display->drawString(25, 21, "ON");
  else
    Heltec.display->drawString(25, 21, "OFF");
  Heltec.display->display();

  Udp.beginPacket(IP_Remote, 1999);
  String message;

  if (activateScent)
    message += "yes";
  else
    message += "no";

  byte buffer[message.length() + 1];
  message.getBytes(buffer, message.length() + 1);
  Udp.write(buffer, sizeof(buffer));

  Udp.endPacket();
}
