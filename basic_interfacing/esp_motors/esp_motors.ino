
// Motor pins
int motor1pin1 = 25;
int motor1pin2 = 26;
int motor2pin1 = 27;
int motor2pin2 = 14;

//....................................motor starting..............................................//
void motor_start(){

    Serial.println("........................motor starting....................");

    analogWrite(32, 40); //ENA pin
    analogWrite(33, 40); //ENB pin

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

  pinMode(motor1pin1, OUTPUT);//motor pins
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  pinMode(32, OUTPUT);// motor drivers 
  pinMode(33, OUTPUT);// motor drivers 

}

void loop() {
  // put your main code here, to run repeatedly:
  motor_start();    
}
