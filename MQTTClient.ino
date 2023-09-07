#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

char ssid[] = "Redmi";    // Your network SSID (name)
char pass[] = "00000000"; // Your network password (WPA or WEP key)

int light = 2; // Digital pin for an LED indicator

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "mqtt-dashboard.com";
int port = 1883;
const char topic[] = "SITMQTT"; // Topic to subscribe to

void setup() {
  Serial.begin(9600);
  pinMode(light, OUTPUT);
  while (!Serial) {
    // Wait for the serial port to connect (needed for native USB port)
    ;
  }

  // Attempt to connect to the WiFi network
  Serial.print("Fetching WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print("Loading SSID.....");
    delay(5000);
  }

  Serial.println("Connection established with the provided network");
  Serial.println();

  Serial.print("Fetching MQTT broker: ");
  Serial.println(broker);

  // Check if the MQTT connection was successful
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed.....");
    Serial.println(mqttClient.connectError());
    Serial.print("Retry Again with the default MQTT port i.e., 1883");
    while (1);
  }

  Serial.println("Fetched MQTT server and established connection successfully.");
  

  Serial.print("Information Subcription: ");
  Serial.println(topic);
  Serial.println();
  mqttClient.subscribe(topic);

  // Uncomment the line below to unsubscribe from a topic:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on subscriber: ");
  Serial.println(topic);
  Serial.println();
}

void loop() {
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    Serial.print("Subscriber recieved messages/alerts successfully.");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length: ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    // Print the message content
    while (mqttClient.available()) {
      Serial.print((char)mqttClient.read());
    }
    Serial.println();

    // Flash the LED as a visual indicator of message reception
    digitalWrite(light, HIGH);
    delay(200);
    digitalWrite(light, LOW);
    delay(200);
    digitalWrite(light, HIGH);
    delay(200);
    digitalWrite(light, LOW);
    delay(200);
    digitalWrite(light, HIGH);
    delay(200);
    digitalWrite(light, LOW);

    Serial.println();
  }
}
