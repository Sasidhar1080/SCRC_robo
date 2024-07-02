#include <DFRobot_DHT11.h>
// #include <Wire.h>
#include "constants.h"


//.........................................pm2,pm10.................................................//

byte command_frame[9] = {0xAA, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x67, 0xBB};
byte received_data[9];
int sum = 0;

void send_command(byte command) {
  command_frame[1] = command;
  sum = command_frame[0] + command_frame[1] + command_frame[2] + command_frame[3] + command_frame[4] + command_frame[5] + command_frame[8];
  int rem = sum % 256;
  command_frame[6] = (sum - rem) / 256;
  command_frame[7] = rem;
  delay(1000);
  Serial.write(command_frame, 9);
}

bool checksum() {
  sum = received_data[0] + received_data[1] + received_data[2] + received_data[3] + received_data[4] + received_data[5] + received_data[8];
  return sum == (received_data[6] * 256 + received_data[7]);
}

void calculate_pm() {
  pm2 = received_data[4] * 256 + received_data[5];
  delay(2000);
  pm10 = received_data[2] * 256 + received_data[3];
  Serial.print("PM2.5: ");
  Serial.println(pm2);
  Serial.print("PM10: ");
  Serial.println(pm10);
  Serial.println("----------PM updated successfully!-------------");
}

void PM_setup() {
  send_command(0x01);
}

void PM_Reading() {
  if (millis() - prev_time > 5000) {
    send_command(0x02);
    prev_time = millis();
  }
  if (Serial.available() >= 9) { // Ensure there are enough bytes available to read
    Serial.readBytes(received_data, 9);
    if (checksum()) {
      calculate_pm();
    }
  }
}

//.........................................temp,rh.................................................//

void dht_read() {
  DHT.read(DHT11_PIN);
  temp = DHT.temperature;
  rh = DHT.humidity;
  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("rh: ");
  Serial.println(rh);
  Serial.println("----------temp & rh updated successfully!-------------");
}

//....................................LDR..............................................//
void LDR_read(){
  val = analogRead(A0); 
  Serial.print("Lux : ");
  Serial.println(val);
  Serial.println("----------LUX  updated successfully!-------------");
}

//....................................CO2..............................................//
void co2_read(){
  co2Value = digitalRead(2);
  Serial.print("CO2: ");
  Serial.println(co2Value ,DEC);
  Serial.println("----------CO2  updated successfully!-------------");
}

//....................................motor starting..............................................//
void motor_start(){

    Serial.println("........................motor starting....................");

    analogWrite(9, 40); //ENA pin
    analogWrite(10, 40); //ENB pin

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

    Serial.println("........................motor ending....................");

}


void setup() {
  Serial.begin(9600);
  Serial.println("start");
  PM_setup();//send pm command

  pinMode(motor1pin1, OUTPUT);//motor pins
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  pinMode(9, OUTPUT);// motor drivers 
  pinMode(10, OUTPUT);// motor drivers 

  pinMode(5, INPUT);//co2 pin
  // pinMode(A0, INPUT);//LDR Pin mode
  delay(10000);
}

void loop() {
  PM_Reading(); // pm2, pm10
  dht_read();   // temp, rh
  co2_read();
  LDR_read();
  motor_start();    
}
