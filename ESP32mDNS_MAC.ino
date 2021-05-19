/*ESP32mDNS_MAC
   Forrest Erickson
   Date: 20210519
   License / Dedication: Public domain, Free to good home.
   Warranty: This program will kill you but is not guarenteed to do so.

   Reads the ESP32 chip ID and puts it into the form of a MAC address.
   Composes a unique mDNS domain name composed of the PRODUCT_NAME and the MAC
   Reports unique mDNS domain name on serial port
   Winks LED

*/

const int BAUDRATE = 115200;            //Use a fast baud.

//For reading MAC
#include <esp_system.h>
uint8_t chipid[8] = {0, 0, 0, 0, 0, 0, 0, 0};
char myMACString[18];
String myMDNSid;

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>

// Program Description
const char* PROGRAM_NAME = "ESPmDNS_MAC";  // This program
const char* PRODUCT_NAME = "widget";  // This device model.
const char* PROGRAM_VERSION = "V 0.0.1";                // First version
const char* PROGRAM_SLOGAN = "Caution! Amused Scientist"; // Caution Amused Scientist, Forret Erickson

//Set LED for Uno or ESP32 Dev Kit on board blue LED.
const int LED_BUILTIN = 2;    // ESP32 Kit
const int HIGH_TIME_LED = 900;
const int LOW_TIME_LED = 100;
long lastLEDtime = 0;
long nextLEDchange = 100; //time in ms.

// TCP server at port 80 will respond to HTTP requests
WiFiServer server(80);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.begin(BAUDRATE);

  //Serial splash message
  // We connect to a WiFi network
  Serial.print("Disconnect any old WiFi connection ");
  WiFi.disconnect();              // Any old sessions.
  delay(100);
  Serial.println(PROGRAM_NAME);
  Serial.println(PROGRAM_VERSION);
  Serial.println(PROGRAM_SLOGAN);
  Serial.println(PRODUCT_NAME);

  //const char* ssid = "............";
  //const char* password = "..............";
  const char* ssid     = "NETGEAR_11N";     // Netgear WAC104 SN: 4SL373BC00087
  const char* password = "Heavybox201";  // Lab wifi router
  //const char* password = "Heavybox202";  // bad pw.

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  //Read and Report MAC as a unique device ID.
  Serial.printf("Chip ID before read MAC: %X\n", chipid);
  esp_efuse_read_mac(chipid);
  Serial.printf("Chip ID: %X\n", chipid);   //This prints correct MAC

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Setup mDNS, with unique ID.
  esp_efuse_read_mac(chipid);
  snprintf(myMACString, sizeof(myMACString), "%02X%02X%02X%02X%02X%02X", chipid[0], chipid[1], chipid[2], chipid[3], chipid[4], chipid[5]);
  //  Serial.println("-myMACString is: " + String(myMACString));
  myMDNSid = PRODUCT_NAME + String(myMACString);
  if (!MDNS.begin(myMDNSid.c_str())) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("My mDNS address: http://" + String(myMDNSid.c_str()) + ".local");
  MDNS.addService("http", "tcp", 80);


  server.begin();               //HTTP server

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED of at end of setup
}

void loop() {
  // put your main code here, to run repeatedly:

  //Wink the LED
  if (((millis() - lastLEDtime) > nextLEDchange) || (millis() < lastLEDtime)) {
    if (digitalRead(LED_BUILTIN) == LOW) {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      nextLEDchange = HIGH_TIME_LED;
    } else {
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      nextLEDchange = LOW_TIME_LED;
    }
    lastLEDtime = millis();
  }//end LED wink

  // SimpleWiFiServer example web page
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<h2>Program: ESP32mDNS_MAC</h2>");
            client.println("My mDNS address: <strong>http://" + String(myMDNSid.c_str()) + ".local</strong>");
            client.print("<br><br>Make QR codes of this mDNS at:");
            client.print("<ul><li>https://www.qrcode-monkey.com/ </li><li> https://www.qr-code-generator.com/</li></ul>");
            
            
            client.print("<hr><h6>" + String(PROGRAM_SLOGAN) + "<br>");
            client.print("Amused Scientist not affiliated or compenseated by QT generators.</h6>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }



}
