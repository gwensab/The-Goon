#include <BTS7960.h>   // motor driver
#include <NewPing.h>  // SR04 distance sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>      // DHT11 temperature sensor  

// declare motor driver consants
const uint8_t R_EN_Right = 8;       // pinout
const uint8_t L_EN_Right = 7;
const uint8_t L_PWM_Right = 6;
const uint8_t R_PWM_Right = 5;
const uint8_t R_EN_Left = 12;
const uint8_t L_EN_Left = 11;
const uint8_t L_PWM_Left = 10;
const uint8_t R_PWM_Left = 9;
BTS7960 motorController_Right(L_EN_Right, R_EN_Right, L_PWM_Right, R_PWM_Right);  // right motor controller configuration
BTS7960 motorController_Left(L_EN_Left, R_EN_Left, L_PWM_Left, R_PWM_Left);       // left motor controller configuration
 
// declare SR04 sensor constants
#define trigger_pin_1 44      // input pinout
#define trigger_pin_2 45      
#define trigger_pin_3 46      
#define echo_pin_1 44         // output pinout
#define echo_pin_2 45         
#define echo_pin_3 46         
#define max_distance 350      // max distance in centimeters, about 140  inches
NewPing sonar1(trigger_pin_1, echo_pin_1, max_distance);      // front distance sensor configuration
NewPing sonar2(trigger_pin_2, echo_pin_2, max_distance);      // right distance sensor configuration
NewPing sonar3(trigger_pin_3, echo_pin_3, max_distance);      // left distance sensor configuration

// declare DHT11 sensor constants
#define DHTPIN 4 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// declare variables

bool roomFront = false;                 // set initial sensor commands
bool roomRight = false;
bool roomLeft = false;
bool moreRoomRight = false;
bool moreRoomLeft = false;
int speed = 150;                        //set initial speed of motors
int timesRun = 0;

void setup() {
  Serial.begin (9600);
  enableDrive();
  dht.begin();
}
void loop() {

    if (timesRun < 1000) {
    printTemp();
    //printDistance();
    checkSensors();
    leftVright();
    sensorCommands();
    timesRun++;
  }
  else{
  disableDrive();
  exit(0);
  } 
}

//DHT11 sensor commands
void printTemp() {
  delay(100);
  float h = dht.readHumidity();           // Read temperature as Celsius (the default)
  float f = dht.readTemperature(true);    // Read temperature as Fahrenheit (isFahrenheit = true)
  //Serial.print("Humidity: ");  
  //Serial.print("%  Temperature: ");
  //Serial.print("°F ");
  Serial.print(f);
  Serial.print(" ");
  Serial.println(h);

 
}

// SR04 sensor commands

/*
void printDistance() {                    // print distance of each sensor in serial monitor
  Serial.print("Front: ");
  Serial.print(sonar1.ping_in());
  Serial.print(" in  ");
  Serial.print("Right: ");
  Serial.print(sonar2.ping_in());
  Serial.print(" in  ");
  Serial.print("Left: ");
  Serial.print(sonar3.ping_in());
  Serial.print(" in  ");
  Serial.println(" ");
}
*/

void  leftVright() {                                //compare distance of left vs right sensor
  if (sonar2.ping_in() > sonar3.ping_in()) {
     moreRoomRight = true;
  }
  if (sonar3.ping_in() > sonar2.ping_in()) {
     moreRoomLeft = true;
  }
}
void checkSensors() {                                                 //account for errors with distance sensor feedback
  if (sonar1.ping_in() > 15 || sonar1.ping_in() < 1) {
    roomFront = true;
  }
  else if(sonar1.ping_in() <= 15 && sonar1.ping_in() >= 1){
    roomFront = false;
  }
  if (sonar2.ping_in() > 10 || sonar2.ping_in() < 1) {
    roomRight = true;
  }
  else if(sonar2.ping_in() <= 10 && sonar2.ping_in() >= 1){
    roomRight = false;
  }
  if (sonar3.ping_in() > 10 || sonar3.ping_in() < 1) {
    roomLeft = true;
  }
  else if(sonar3.ping_in() <= 10 && sonar3.ping_in() >= 1){
    roomLeft = false;
  } 
}
void sensorCommands() {
  if (roomFront == true && roomRight == true && roomLeft == true) {
    forward();
  }  
  if (roomFront == true && roomRight == true && roomLeft == false) {
    turnRight();
    forward();
  }
  if (roomFront == true && roomRight == false && roomLeft == true) {
    turnLeft();
    forward();
  }  
  if (roomFront == false && roomRight == true && roomLeft == true) {          //when left and right have same bool value, check leftVright
     if (moreRoomRight == true) {
      backward();
      turnRight();
    }
    else if (moreRoomLeft == true) {
      backward();
      turnLeft();
    }
    else{
    backward();
    }
  }    
  if (roomFront == true && roomRight == false && roomLeft == false) {
    if (moreRoomRight == true) {
      turnRight();
      forward();
    }
    else if (moreRoomLeft == true) {
      turnLeft();
      forward();
    }
    else{
    forward();
    }
  }
  if (roomFront == false && roomRight == true && roomLeft == false) {
    backward();
    turnRight();
  }    
  if (roomFront == false && roomRight == false && roomLeft == true) {
    backward();
    turnLeft();
  }  
  if (roomFront == false && roomRight == false && roomLeft == false) {
    if (moreRoomRight == true) {
      backward();
      turnRight();
    }
    else if (moreRoomLeft == true) {
      backward();
      turnLeft();
    }
    else{
    backward();
    }
  }  
}

// motor driver commands
void enableDrive() {                          // use at beginning of program
  motorController_Right.Enable();             // enable motor driver controlling right side
  motorController_Left.Enable();              // enable motor driver controlling left side
  //Serial.println("Motor Drivers Enabled");
}
void forward() {
   motorController_Right.TurnLeft(speed);      // motors on right side of bot rotate clockwise
  motorController_Left.TurnRight(speed);      // motors on left side of bot rotate clockwise
  //Serial.println("Moving forward");
  delay(300);
}
void backward() {
  motorController_Right.TurnRight(speed);     // motors on right side of bot rotate counterclockwise
  motorController_Left.TurnLeft(speed);       // motors on left side of bot rotate counterclockwise
  //Serial.println("Moving backward");
  delay(300);
}
void turnLeft() {
  motorController_Right.TurnLeft(speed);      // motors on right side of bot rotate clockwise
  motorController_Left.TurnLeft(speed);       // motors on left side of bot rotate counterclockwise
  //Serial.println("Turning left");
  delay(150);
}
void turnRight() {
  motorController_Right.TurnRight(speed);     // motors on right side of bot rotate counterclockwise
  motorController_Left.TurnRight(speed);      // motors on left side of bot rotate clockwise
  //Serial.println("Turning right");
  delay(150);
}
void pause() {
  motorController_Right.Stop();               // motors on right side of bot fully stop
  motorController_Left.Stop();                // motors on left side of bot fully stop
  //Serial.println("Pause");
  delay(100);
}
void disableDrive() {                         // use at the end of the program
  motorController_Right.Stop();               // motors on right side of bot fully stop
  motorController_Left.Stop();                // motors on left side of bot fully stop
  motorController_Right.Disable();            // disable motor driver controlling right side
  motorController_Left.Disable();             // disable motor driver controlling left side
  //Serial.println("Motors stopped");
  }
