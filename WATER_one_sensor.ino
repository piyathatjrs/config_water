#include "DHT.h"
#include <WiFi.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include<PubSubClient.h>



#define WIFI_SSD "กรอก SSID"
#define WIFI_PASS "กรอก password"

#define MQTT_SERVER   "tailor.cloudmqtt.com"
#define MQTT_PORT       14090
#define MQTT_USERNAME "zpfdcyrx"
#define MQTT_PASSWORD "OypjtCmtYhqp"
#define MQTT_NAME     "ESSSSS"

WiFiClient client;

PubSubClient mqtt(client);

void callback(char* topic, byte* payload, unsigned int length) {
  String topic_str = "";
  String payload_str = "";
  for (int j = 0 ; j < strlen(topic) ; j++) {
    topic_str += topic[j];
  }
  for (int i = 0 ; i < length ; i++) {
    Serial.print((char)payload[i]);
    payload_str += (char)payload[i];
  }

  Serial.print("[ ");
  Serial.print(topic);
  Serial.print(" ]");
  Serial.print(payload_str);

}

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  pinMode(LED_BUILTIN, OUTPUT);


  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSD);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSD, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);

  connectMQTT();

}

//water
const int AirValue = 0;   //you need to replace this value with Value_1
const int WaterValue = 2600;  //you need to replace this value with Value_2
int sensorValue = analogRead(32); 

void loop() {

int sensorValue_water = analogRead(32);
int map_value_water=0;
Serial.println(sensorValue_water) ;
map_value_water = map(sensorValue_water, AirValue, WaterValue, 0, 100);  
if(sensorValue_water >=2600){
   Serial.println(sensorValue_water); 
   Serial.println("100"); 
    Publish("TEST/MQTT_WATER", ("100"));
  }  else if(sensorValue_water <=0 ){
     Serial.println(sensorValue_water); 
   Serial.println("0"); 
    Publish("TEST/MQTT_WATER", ("0"));
  } else {
    Serial.println(sensorValue_water); 
    Serial.print(map_value_water);
     Publish("TEST/MQTT_WATER", String(map_value_water));
    Serial.print(" %\n===============================\n");
    }
     
  Serial.println(); 
  delay(1500);



}

void connectMQTT() {
  Serial.print("MQTT connect... ");
  if (mqtt.connect(MQTT_NAME , MQTT_USERNAME , MQTT_PASSWORD )) {
    Serial.print("success");
  } else {
    Serial.print("Failed");
    delay(5000);
  }
  Serial.println();

}

void Publish(String key , String message) {
  char topic[100] , payload[100];
  key.toCharArray(topic, 100);
  message.toCharArray(payload, 100);
  mqtt.publish(topic , payload);
}
int analog_value = 0;
double temp = 0;
