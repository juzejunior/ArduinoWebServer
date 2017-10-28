#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177); // Arduino IP Add
EthernetServer server(3003); // Web server

#define PIN_LED 13

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT); // Set Pin 13 to OUTPUT Mode
  Serial.print("Starting...");
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {
  // Is there a client (Our Android smartphone)
  EthernetClient client = server.available(); 
  
  if (client) {
   Serial.println("Client connected");
    //an http request ends with a blank line
    boolean currentLineIsBlank = true;

    while (client.connected()) {
      while(client.available()) {
        char c = client.read();
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        if (c == '\n' && currentLineIsBlank) {
            while(client.available())
            {
              int c = client.read();  
              if(c == '1') {
                Serial.println("Turn On LED");
                digitalWrite(PIN_LED, HIGH);
              }else {
                Serial.println("Turn Down LED");
              }
            }
            Serial.println();
           //Serial.println("Sending response");
           // send a standard http response header
           client.println("HTTP/1.0 200 OK");
           client.println("Content-Type: text/html");
           client.println("Connection: keep-alive");
           client.println();
           client.stop();
        }
        else if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    Serial.println();
  }
  // Stay a half of second  
  delay(300);
}
