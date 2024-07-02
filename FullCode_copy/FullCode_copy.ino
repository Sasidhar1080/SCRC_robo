#include <Wire.h>
#include <DFRobot_SGP40.h>
#include <Adafruit_SH110X.h>
#include <DFRobot_DHT11.h>

int motor1pin1 = 8;
int motor1pin2 = 7;

int motor2pin1 = 6;
int motor2pin2 = 5;

DFRobot_SGP40 mySgp40;
 
#define i2c_Address 0x3c
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//air quality 
// int pm2 = 0;
// int pm10 = 0;

// unsigned long previous_loop, previous_10, previous_25, prev_time;

// byte command_frame[9] = { 0xAA, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x67, 0xBB };
// byte received_data[9];
// int sum = 0;
// void send_command(byte command) {
//   command_frame[1] = command;
//   int sum = command_frame[0] + command_frame[1] + command_frame[2] + command_frame[3] + command_frame[4] + command_frame[5] + command_frame[8];
//   int rem = sum % 256;
//   command_frame[6] = (sum - rem) / 256;
//   command_frame[7] = rem;
//   delay(1000);
//   Serial.write(command_frame, 9);
// }

// bool checksum() {
//   sum = int(received_data[0]) + int(received_data[1]) + int(received_data[2]) + int(received_data[3]) + int(received_data[4]) + int(received_data[5]) + int(received_data[8]);
//   if (sum == ((int(received_data[6]) * 256) + int(received_data[7]))) {
//     return true;
//   } else
//     return false;
// }
// void calculate_pm() {
//   pm2 = int(received_data[4]) * 256 + int(received_data[5]);
//   delay(2000);
//   pm10 = int(received_data[2]) * 256 + int(received_data[3]);
//   Serial.print("PM2.5: ");
//   Serial.println(pm2);
//   Serial.print("PM10: ");
//   Serial.println(pm10);
//   }

// void PM_setup() {
//   send_command(0x01);
// }

// void PM_Reading() {
//   delay(5000);
//   if (millis() - prev_time > 5000) {
//     send_command(0x02);
//     prev_time = millis();
//   }
//   if (Serial.available()) {
//     Serial.readBytes(received_data, 9);
//     if (checksum()) {
//       calculate_pm();
//     }
//   }
// }
//////////////
void TCA9548A(uint8_t bus) //function of TCA9548A
{
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}

uint16_t index;
uint16_t vocIndex;
// Calibration data (VOC index, CO2 concentration)
struct CalibrationData
{
  uint16_t vocIndex;
  uint16_t co2Concentration;
};

CalibrationData calibrationData[] ={
  {100, 400},   // Example data point: VOC index 100 corresponds to CO2 concentration 400 ppm
  // Add more calibration data points here
};

int numCalibrationPoints = sizeof(calibrationData) / sizeof(CalibrationData);

// Function to calculate estimated CO2 concentration based on VOC index
  uint16_t estimateCO2(uint16_t vocIndex) {
  uint16_t minVoc = calibrationData[0].vocIndex;
  uint16_t maxVoc = calibrationData[numCalibrationPoints - 1].vocIndex;

  uint16_t minCO2 = calibrationData[0].co2Concentration;
  uint16_t maxCO2 = calibrationData[numCalibrationPoints - 1].co2Concentration;

  if (vocIndex <= minVoc) {
    return minCO2;
  } else if (vocIndex >= maxVoc) {
    return maxCO2;
  } else {
    float slope = (maxCO2 - minCO2) / (float)(maxVoc - minVoc);
    return minCO2 + (vocIndex - minVoc) * slope;
  }
}

#define DHT11_PIN 2
DFRobot_DHT11 DHT;
int val = 0 ;  

void  setup()
{
  Serial.begin(9600);
  Serial.print("start");
  Wire.begin();

  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  pinMode(A0,INPUT);  // LDR Sensor output pin connected  
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT); 

  display.begin(i2c_Address, true); // Address 0x3C default
  //PM_setup();
  delay(3000);
  Serial.print("done");

  display.display();

}


void loop() 
{
  // delay(3000);
  // PM_Reading();
  // Serial.print("pm10");
  // Serial.println(pm10);
  analogWrite(9, 40); //ENA pin
  analogWrite(10, 40); //ENB pin
  
  DHT.read(DHT11_PIN);

  TCA9548A(1);
  mySgp40.begin();
  index = mySgp40.getVoclndex();
  Serial.println(index);
  uint16_t estimatedCO2 = estimateCO2(vocIndex);
  Serial.print(estimatedCO2);
  Serial.println("PPm");

  val = analogRead(A0);
  Serial.print("Lux : ");
  Serial.println(val);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(25, 0);
  display.println("Ambience Robo");

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 10);
  display.println("VOC ");  
  display.setCursor(65,10);
  display.println(index); 

  // digitalWrite(motor1pin1, LOW);
  // digitalWrite(motor1pin2, HIGH);

  // digitalWrite(motor2pin1, LOW);
  // digitalWrite(motor2pin2, LOW);

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 20);
  display.println("CO2 level");  
  display.setCursor(65,20);
  display.println(estimatedCO2); 
  display.setCursor(85,20);
  display.println("PPm"); 
  
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 30);
  display.println("Temp");  
  display.setCursor(65,30);
  display.println(DHT.temperature); 
  display.setCursor(85,30);
  display.println("C"); 


  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 40);
  display.println("Humi");  
  display.setCursor(65,40);
  display.println(DHT.humidity); 
  display.setCursor(85,40);
  display.println("%"); 

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 50);
  display.println("LUX ");  
  display.setCursor(65,50);
  display.println(val);

  // display.setTextSize(1);
  // display.setTextColor(SH110X_WHITE);
  // display.setCursor(0, 50);
  // display.println("PM2.5:");  
  // display.setCursor(45,50);
  // display.println(pm2);
  // display.setCursor(65, 50);
  // display.println("PM10:");  
  // display.setCursor(100,50);
  // display.println(pm10);

  display.display();

  // digitalWrite(motor1pin1, HIGH);
  // digitalWrite(motor1pin2, LOW);

  // digitalWrite(motor2pin1, HIGH);
  // digitalWrite(motor2pin2, LOW);
  
  // delay(3000);

  // digitalWrite(motor1pin1, LOW);
  // digitalWrite(motor1pin2, HIGH);

  // digitalWrite(motor2pin1, LOW);
  // digitalWrite(motor2pin2, HIGH);
  // delay(3000);

  
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);

  delay(5000);

  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);

  delay(5000);

  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
   
}
