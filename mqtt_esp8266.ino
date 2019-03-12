
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define WLAN_SSID       "You_cant_hack_me"
#define WLAN_PASS       "1234567890"



#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Kavish_Mehta"
#define AIO_KEY         "dc54fb0b6d134135b75d45311fa3de94"



.
WiFiClient client;
.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


Adafruit_MQTT_Publish btn = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/feed1");


Adafruit_MQTT_Subscribe onf = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led");



void MQTT_connect();
int led1 = D7;
int btn1 = D6;
int reads;
void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onf);
  pinMode(led1,OUTPUT);
  pinMode(btn1,INPUT);
}

uint32_t x=0;

void loop() {

  MQTT_connect();

 
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onf) {
      Serial.print(F("Got: "));
      Serial.println((char *)onf.lastread);
    }
  }
  if(strcmp((char*)onf.lastread,"ON")== 0)
  digitalWrite(led1,HIGH);

  
 reads = digitalRead(btn1);
 delay(200);
 if(reads == 1)
 {
   Serial.print(reads);
  Serial.print("...");
  if (! btn.publish(reads)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
 }

 
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
        
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
