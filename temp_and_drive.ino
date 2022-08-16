/*
@gwensab last edited 8/16/2022
This is the code for Arduino Mega 2560 board. Code includes interfacing with two motor drivers, one temperature sensor, and three distance sensors. 
Code has the capacity to gather information from sensors and drive according to that information.
*/

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
#define trigger_pin_1 44      // set input pins
#define trigger_pin_2 45      
#define trigger_pin_3 46      
#define echo_pin_1 44         // set output pins
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
int speed = 150;                        // set initial speed of motors
int timesRun = 0;


void setup() {
  Serial.begin (9600);
  enableDrive();
  dht.begin();
}
void loop() {

    if (timesRun < 10000) {             // run 10000 times
    printTemp();
    //printDistance();                  // commented out to keep serial monitor clear, use when testing arduino driving ect.
    checkSensors();
    leftVright();
    sensorCommands();
    timesRun++;
  }
  else{                                 // stop after run 10000 times
  disableDrive();
  exit(0);
  } 
}

//DHT11 sensor commands
void printTemp() {
  delay(100);
  float h = dht.readHumidity();           // read humidity
  float f = dht.readTemperature(true);    // read temperature as Fahrenheit (isFahrenheit = true)
  //Serial.print("Temperature: ");        // commented out to keep serial monitor clear, use when testing arduino driving ect.
  Serial.print(f);                        // print temperature serial monitor
  //Serial.print("°F  Humidity:");        // commented out to keep serial monitor clear, use when testing arduino driving ect.
  Serial.print(" ");                      // print space between the two values
  Serial.println(h);                      // print humidity to serial monitor
  //Serial.print("%");                    // commented out to keep serial monitor clear, use when testing arduino driving ect.

 
}

// SR04 sensor commands

/*
void printDistance() {                    // print distance of each sensor in serial monitor
  Serial.print("Front: ");                // commented out to keep serial monitor clear, use when testing arduino driving ect.
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
  if (sonar2.ping_in() > sonar3.ping_in()) {        // if right sensor value is greater than left sensor value
     moreRoomRight = true;                          // there is more room on the right
  }
  if (sonar3.ping_in() > sonar2.ping_in()) {        // if left sensor value is greater than right sensor value
     moreRoomLeft = true;                           // there is more room on the left
  }
}
void checkSensors() {                                                 // account for errors with distance sensor feedback
  if (sonar1.ping_in() > 15 || sonar1.ping_in() < 2) {                // if front sensor value is more than 15 or less than 2
    roomFront = true;                                                 // there is room in front
  }
  else if(sonar1.ping_in() <= 15 && sonar1.ping_in() >= 2){           // if front sensor value is between 15 and 2
    roomFront = false;                                                // there is not room in front
  }
  if (sonar2.ping_in() > 10 || sonar2.ping_in() < 2) {                // if right sensor value is more than 15 or less than 2
    roomRight = true;                                                 // there is room to right
  }
  else if(sonar2.ping_in() <= 10 && sonar2.ping_in() >= 2){           // if right sensor value is between 15 and 2
    roomRight = false;                                                // there is not room to right
  }
  if (sonar3.ping_in() > 10 || sonar3.ping_in() < 2) {                // if left sensor value is more than 15 or less than 2
    roomLeft = true;                                                  // there is room to left
  }
  else if(sonar3.ping_in() <= 10 && sonar3.ping_in() >= 2){           // if left sensor value is between 15 and 2
    roomLeft = false;                                                 // there is not room to left
  } 
}
void sensorCommands() {
  if (roomFront == true && roomRight == true && roomLeft == true) {           // when room in all directions, drive forward
    forward();
  }  
  if (roomFront == true && roomRight == true && roomLeft == false) {          // when room front and right but not left, turn right and go forward
    turnRight();
    forward();
  }
  if (roomFront == true && roomRight == false && roomLeft == true) {          // when room front and left but not right, turn left and go forward
    turnLeft();
    forward();
  }  
  if (roomFront == false && roomRight == true && roomLeft == true) {          // when left and right have same bool value, check leftVright
     if (moreRoomRight == true) {                                             // when more room right, back up and turn right
      backward();
      turnRight();
    }
    else if (moreRoomLeft == true) {                                          // when more room left, back up and turn left
      backward();
      turnLeft();
    }
    else{                                                                     // when neither is true, just back up
    backward();
    }
  }    
  if (roomFront == true && roomRight == false && roomLeft == false) {         // when left and right have same bool value, check leftVright
    if (moreRoomRight == true) {                                              // when more room right, turn right and go forward
      turnRight();
      forward();
    }
    else if (moreRoomLeft == true) {                                          // when more room left, turn left and go forward 
      turnLeft();
      forward();
    }
    else{                                                                     // when neither is true, just go forward
    forward();
    }
  }
  if (roomFront == false && roomRight == true && roomLeft == false) {         // when room right but not front or left, back up and turn right
    backward();
    turnRight();
  }    
  if (roomFront == false && roomRight == false && roomLeft == true) {         // when room left but not from or right, back up and turn left
    backward();
    turnLeft();
  }  
  if (roomFront == false && roomRight == false && roomLeft == false) {        // when left and right have same bool value, check leftVright
    if (moreRoomRight == true) {                                              // when more room right, back up and turn right   
      backward();
      turnRight();
    }
    else if (moreRoomLeft == true) {                                          // when more room left, back up and turn left
      backward();
      turnLeft();
    }
    else{                                                                     // when neither is true, just back up 
    backward();
    }
  }  
}

// motor driver commands
void enableDrive() {                           // use at beginning of program
  motorController_Right.Enable();              // enable motor driver controlling right side
  motorController_Left.Enable();               // enable motor driver controlling left side
  //Serial.println("Motor Drivers Enabled");   // commented out to keep serial monitor clear, use when testing arduino driving ect.
}
void forward() {
   motorController_Right.TurnLeft(speed);      // motors on right side of bot rotate clockwise
  motorController_Left.TurnRight(speed);       // motors on left side of bot rotate clockwise
  //Serial.println("Moving forward");          // commented out to keep serial monitor clear, use when testing arduino driving ect.
  delay(300);
}
void backward() {
  motorController_Right.TurnRight(speed);      // motors on right side of bot rotate counterclockwise
  motorController_Left.TurnLeft(speed);        // motors on left side of bot rotate counterclockwise
  //Serial.println("Moving backward");         // commented out to keep serial monitor clear, use when testing arduino driving ect.
  delay(300);
}
void turnLeft() {
  motorController_Right.TurnLeft(speed);       // motors on right side of bot rotate clockwise
  motorController_Left.TurnLeft(speed);        // motors on left side of bot rotate counterclockwise
  //Serial.println("Turning left");            // commented out to keep serial monitor clear, use when testing arduino driving ect.
  delay(150);
}
void turnRight() {
  motorController_Right.TurnRight(speed);      // motors on right side of bot rotate counterclockwise
  motorController_Left.TurnRight(speed);       // motors on left side of bot rotate clockwise
  //Serial.println("Turning right");           // commented out to keep serial monitor clear, use when testing arduino driving ect.
  delay(150);
}
void pause() {
  motorController_Right.Stop();                // motors on right side of bot fully stop
  motorController_Left.Stop();                 // motors on left side of bot fully stop
  //Serial.println("Pause");                   // commented out to keep serial monitor clear, use when testing arduino driving ect.
  delay(100);
}
void disableDrive() {                          // use at the end of the program
  motorController_Right.Stop();                // motors on right side of bot fully stop
  motorController_Left.Stop();                 // motors on left side of bot fully stop
  motorController_Right.Disable();             // disable motor driver controlling right side
  motorController_Left.Disable();              // disable motor driver controlling left side
  //Serial.println("Motors stopped");          // commented out to keep serial monitor clear, use when testing arduino driving ect.
  }
