#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


const char* ssid = "HP Victus";
const char* password = "CHIRAGSONI";
const char* udpServerIP = "192.168.133.48";
const int udpServerPort = 6000;
WiFiUDP Udp;

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Initialize MPU6050 sensor
  Serial.println("Initializing MPU6050...");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  Serial.println("MPU6050 initialized");
  Udp.begin(udpServerPort);
  delay(10);
}

void loop() {
  // Get sensor data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Prepare data to send
  String sensorData = String(a.acceleration.x) +"," + String(a.acceleration.y) +
                     "," + String(a.acceleration.z) + "," + "0"
                     "," + String(g.gyro.x) +
                     "," + String(g.gyro.y) +
                     "," + String(g.gyro.z);

  // Send sensor data via UDP
  Udp.beginPacket(udpServerIP, udpServerPort);
  Udp.write(sensorData.c_str());
  Udp.endPacket();

  // Print sensor data to Serial monitor
  Serial.println(sensorData);

  delay(5); // Adjust the delay based on your requirements
}