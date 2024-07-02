// dht11
#define DHT11_PIN 2
DFRobot_DHT11 DHT;
int temp, rh;

// Air quality variables
int pm2 = 0;
int pm10 = 0;
unsigned long prev_time = 0; // Added initialization

// co2, vocindex
DFRobot_SGP40 mySgp40;
uint16_t index;
uint16_t estimatedCO2; // Added semicolon at the end

//  LDR 
int val = 0;

// Motor pins
int motor1pin1 = 8;
int motor1pin2 = 7;
int motor2pin1 = 6;
int motor2pin2 = 5;

// oled

#define OLED_SDA A4
#define OLED_SCL A5

#define i2c_Address 0x3c 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
