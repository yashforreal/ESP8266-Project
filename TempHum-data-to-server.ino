#include <DHTesp.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

#ifndef STASSID
#define STASSID "YOUR_SSID"
#define STAPSK  "SSID_PASSWORD"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

//Replace the ngrok url here
const char* host = "9d3baeb17776.ngrok.io";
const uint16_t port = 80;

DHTesp dht;
HTTPClient http;

void setup() {
  Serial.begin(115200);
  dht.setup(0, DHTesp::DHT11); // Connect DHT sensor to GPIO 0 i.e D3
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to " + String(ssid));
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop()
{

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

 //Serial.println(temp1);

String data;
 
StaticJsonDocument<200> doc;

float temp1 = dht.getTemperature();
float humidity = dht.getHumidity();

doc["temperature"]=String(temp1);
doc["humidity"]=String(humidity);
doc["lat"]=String(22.789);
doc["long"]=String(78.365);
serializeJson(doc, data);

Serial.println(data); //print temp in json format on serial monitor
delay(5000);
HTTPClient http;
 if(isnan(temp1)){
  Serial.println("Failed to send because data is not a number");
  }
 else{
  delay(2000);
  http.begin("http://9d3baeb17776.ngrok.io/putdata");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(data);
  Serial.println(httpCode);
  http.end();
 }
 
}
