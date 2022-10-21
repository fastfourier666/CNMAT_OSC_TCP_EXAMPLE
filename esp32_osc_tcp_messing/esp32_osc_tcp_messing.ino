#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCData.h>
#include <OSCMessage.h>
#include "SLIPEncodedTCP.h"
WiFiClient client;

const char* ssid     = ".........";
const char* password = ".........";

IPAddress server (192, 168, 42, 86);    // <-----put the server's address here
uint16_t port = 12345;
SLIPEncodedTCP slip (client);
uint32_t t = 0;
OSCMessage incoming;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  /*
   * Every second, send something to the server or try to reconnect
   */
  if (millis() - t >= 1000) {
    t = millis();
    if (client.connected())
    {
      Serial.printf("connnected - sending data\n");
      // Make a HTTP request:
      OSCMessage m ("/hello");
      m.add ((int32_t)666);
      slip.beginPacket();
      m.send (slip);
      slip.endPacket();
      m.empty();
    } else {
      client.connect (server, port);
    }
  }

  /*
   * read incoming data from the server
   */
  if (slip.available())
    while (!slip.endofPacket())
      while (slip.available())
        incoming.fill(slip.read());

  /*
   * Process complete message
   */
  if (!incoming.hasError() && incoming.size() > 0) {
    Serial.printf ("size=%i\n", incoming.size());
    Serial.printf ("good message! %i\n", incoming.getInt(0));
    incoming.empty();
  }
}
