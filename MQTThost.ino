#include <ArduinoMqttClient>

// Include the appropriate WiFi library based on the Arduino board type
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

// Define your WiFi network credentials
char ssid[] = "Redmi";      // Your WiFi network SSID (name)
char pass[] = "00000000";   // Your WiFi network password



// Define pins for the ultrasonic sensor
const int trigPin = 2;
const int echoPin = 3;

float time, path;   // Variables for storing path measurements

WiFiClient wifiClient;      // WiFi client object
MqttClient mqttClient(wifiClient); // MQTT client object

// MQTT broker configuration
const char broker[] = "broker.mqttdashboard.com"; // MQTT broker address
int port = 1883;           // MQTT broker port
const char topic[] = "SITMQTT"; // MQTT topic

const long pause = 1000; // Measurement pause in milliseconds
unsigned long lastmessage = 0;

int track = 0; // tracker for tracking measurements

void setup() {
  // Initialize serial communication and wait for the port to open
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for the serial port to connect (only for boards with native USB port)
  }

  // Attempt to connect to the WiFi network
  Serial.print("Fetching WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // WiFi connection failed, retrying
    Serial.print("Loading SSID.....");
    delay(5000); // Delay before retrying
  }

  Serial.println("Connection established with the provided network");
  Serial.println();

  // Optionally set a unique MQTT client ID and provide username/password for authentication

  // Attempt to connect to the MQTT broker
  Serial.print("Fetching MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed.....");
    Serial.print("Retry Again with the default MQTT port i.e., 1883");
    

    while (1); // Enter an infinite loop if MQTT connection fails
  }

  Serial.println("Fetched MQTT server and established connection successfully.");
  
}

void loop() {
  // Maintain the MQTT connection and handle MQTT messages
  mqttClient.poll();

  // Implement a timer mechanism to take path measurements at regular intervals
  unsigned long messagecurrent = millis();

  if (messagecurrent - lastmessage
 >= pause) {
    // Save the last time a message was sent
   lastmessage
   = messagecurrent;

    // Trigger the ultrasonic sensor to measure path
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure the time of the pulse and calculate the path
    time = pulseIn(echoPin, HIGH);
    path = (time * 0.0343) / 2;

    Serial.print("path: ");
    Serial.println(path);

    if (path < 12) {
      // Send an MQTT message with path information
      mqttClient.beginMessage(topic);
      mqttClient.print("Mayank Nishant : MQTT Wave is detected, ");
      mqttClient.print("path: ");
      mqttClient.print(path);
      mqttClient.endMessage();
      delay(1000); // Delay before sending another MQTT message
    }

    Serial.println();
    track++;
  }
}
