#include <Arduino.h>

const int analogPin = 14;  // Analog pin connected to MQ135 sensor
const float VCC = 3.3;     // Supply voltage for MQ135 sensor
const float R0 = 10000.0;  // Sensor resistance at clean air (in ohms)

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Read analog voltage from MQ135 sensor
  int sensorValue = analogRead(analogPin);
  Serial.print(sensorValue);
  // Calculate the resistance of the sensor
  float sensorResistance = (VCC * 4095.0) / sensorValue - 1;
  
  // Calculate the CO2 concentration in ppm using a simple linear equation
  float ppm = sensorResistance / R0;

  // Print the CO2 concentration to the serial monitor
  Serial.print("CO2 Concentration: ");
  Serial.print(ppm);
  Serial.println(" ppm");

  delay(5000); // Wait for 5 seconds before the next reading
}
