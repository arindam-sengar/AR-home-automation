// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put any  SSID & Password */
const char* ssid = "arindam's wifi";  // Enter SSID here
const char* password = "ksiolajidebt";  //Enter Password here (min 8 characters)

/* Set  IP Address details */
IPAddress local_ip(192,168,3,1);
IPAddress gateway(192,168,3,1);
IPAddress subnet(255,255,255,0);

// Set webserver port number to 80
ESP8266WebServer server(80);

uint8_t LED1pin = D0;
bool LED1 = LOW;
 
uint8_t LED2pin = D1;
bool LED2 = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
 
  //Connect to your  wi-fi network
WiFi.begin(ssid, password);
delay(100);
 
//check wi-fi is connected to wi-fi network
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print(".");
}
   
  server.on("/",OnConnect);
  server.on("/led1on",led1on);
  server.on("/led1off",led1off);
  server.on("/led2on",led2on);
  server.on("/led2off",led2off);
  server.onNotFound(NotFound);
   
  server.begin();
  Serial.println("HTTP server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
  if(LED1)
    digitalWrite(LED1pin, HIGH);
  else
    digitalWrite(LED1pin, LOW);
   
  if(LED2)
    digitalWrite(LED2pin, HIGH);
  else
    digitalWrite(LED2pin, LOW);
}

void update_status(){
  server.send(200, "text/html", SendHTML(LED1,LED2));  // we send HTML page alng with updated LED status.
}

void OnConnect() {
  LED1 = LOW;
  LED2 = LOW;
  update_status();
}

void led1on() {
  LED1 = HIGH;
  update_status();
}
 
void led1off() {
  LED1 = LOW;
  update_status();
}
 
void led2on() {
  LED2 = HIGH;
  update_status();
}
 
void led2off() {
  LED2 = LOW;
  update_status();
}

void NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Device Control</title>\n";
  ptr +="<style>html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #343444;margin: 50px auto 30px;} h3 {color: #434444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 100px;background-color: #1abc9c;border:none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 50%;}\n";
  ptr +=".button-on {background-color: #ff9900;}\n";
  ptr +=".button-on:active {background-color: #996633;}\n";
  ptr +=".button-off {background-color: #003399;}\n";
  ptr +=".button-off:active {background-color: #003366;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 based Arduino Web Server</h1>\n";

  if(led1stat)
     ptr +="<p>Device1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
 else
     ptr +="<p>Device2 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
 
 if(led2stat)
     ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";
 else
     ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
