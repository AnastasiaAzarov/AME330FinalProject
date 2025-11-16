/*
  Feather Sense Analog Pin Reader with Distance - Unity Serial Output
  
  FORMAT: Sends four values as a single comma-separated line (CSV) 
  followed by a newline character (\n).
  Example: "490,512,501,15\n" (A0, A2, A3, Distance_cm)
*/
#include <bluefruit.h>
#include "SR04.h" // Include library for the SR04 Ultrasonic Sensor

// --- Analog Pin Declarations (Feather Sense) ---
const int UpDown = A0;      // A0 Reading
const int FrontBack = A2;   // A2 Reading
const int LeftRight = A3;   // A3 Reading

// --- SR04 Ultrasonic Sensor Pin Declarations ---
#define TRIG_PIN 13
#define ECHO_PIN 12
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long distance_cm;

// Data rate: 50ms interval = 20 updates per second
const long READ_INTERVAL_MS = 50; 
long lastReadTime = 0;

void setup() {
  // Initialize serial communication. Rate must match Unity's SerialPort setting.
  Serial.begin(9600); 
  
  // Wait for serial port to connect. Needed for native USB port devices.
  while (!Serial); 
  
  // Indicate to Unity what data is being sent.
  Serial.println("UNITY_READY: A0, A2, A3, Distance_cm");
}

void loop() {
  // Check if the read interval has passed
  if (millis() - lastReadTime >= READ_INTERVAL_MS) {
    lastReadTime = millis();
    
    // 1. Read analog values (0-1023)
    int valueA0 = analogRead(UpDown);
    int valueA2 = analogRead(FrontBack);
    int valueA3 = analogRead(LeftRight);
    
    // 2. Read distance from the SR04 sensor (in cm)
    // Note: The SR04 library's Distance() function returns 'long'
    distance_cm = sr04.Distance();

    // 3. Send the CSV data packet: A0,A2,A3,Distance_cm\n
    
    // Start the data packet with the first three analog values
    Serial.print(valueA0);
    Serial.print(",");
    Serial.print(valueA2);
    Serial.print(",");
    Serial.print(valueA3);
    Serial.print(",");
    
    // Finish the data packet with the distance value.
    // Serial.println() adds the newline character (\n) that Unity uses as a packet terminator.
    Serial.println(distance_cm); 
  }
}