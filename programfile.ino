#include <ESP8266WiFi.h>
#include <charansai.h>

#define S3 D0
#define S2 D1
#define S1 D2
#define S0 D3
#define analogpin A0

String apiWritekey = WRITE_KEY;
const char* ssid = WIFI_NAME;
const char* password = WIFI_PASS;

const char* server = "api.thingspeak.com";
WiFiClient client;

void setup(){
  Serial.begin(115200);
  delay(10);
  pinMode(analogpin, INPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(alarm, OUTPUT);
  
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to...");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() !=WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Sucessfully connected to");
  Serial.println(ssid);
  Serial.println();
}

void loop() {
  digitalWrite(S0,LOW);
  digitalWrite(S1,LOW);
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  float temp = (analogRead(analogpin) * 330.0) / 1023.0;

  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  int smoke = analogRead(analogpin);

  digitalWrite(S0,LOW);
  digitalWrite(S1,HIGH);
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  int light = analogRead(analogpin);

  if (client.connect(server,80)) {
    String tsData = apiWritekey;
    tsData +="&field1=";
    tsData += String(temp);
    tsData +="&field2=";
    tsData += String(smoke);
    tsData +="&field3=";
    tsData += String(light);
    tsData += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiWritekey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("Smoke Sensor: ");
    Serial.print(smoke);
    Serial.print("Light Sensor: ");
    Serial.print(light);
    Serial.println("Data Uploaded To Thingspeak Server");    
  }
  client.stop();

  Serial.println("Refreshing Server....");
  Serial.println();
  delay(15000);
}
