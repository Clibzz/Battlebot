#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_VL53L0X.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_VL53L0X.h"

/* Motor config */
int motorPinLA = 5; //Linkerwiel achteruit
int motorPinLV = 18; //Linkerwiel vooruit
int motorPinRV = 16; //Rechterwiel vooruit
int motorPinRA = 17; //Rechterwiel achteruit

/* IR config */
int IRPinR = 34;// IR pin rechts
int IRPinL = 39;// IR pin links
int pinR = 0;
int pinL = 0;
int colorThreshold = 2700;

boolean drive = true;

Adafruit_SSD1306 display(128, 32, &Wire, 4);

void setup(){
  Serial.begin(9600);
  pinMode(motorPinRA, OUTPUT);
  pinMode(motorPinRV, OUTPUT);
  pinMode(motorPinLV, OUTPUT);
  pinMode(motorPinLA, OUTPUT);

  /* The display */
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop(){
  display.clearDisplay();
  display.setCursor(0,0); // Start displaying at the top-left corner
  display.println("PinR: " + String(analogRead(IRPinR)));
  display.println("PinR: " + String(analogRead(IRPinL)));
  display.display();
  
  /* Read the IR pins */
  int pinL = analogRead(IRPinL) / colorThreshold;
  int pinR = analogRead(IRPinR) / colorThreshold;

  /* Stand still when the robot sees black on both pins */
  if (pinL >= 1 && pinR >= 1) {
     standStill();
    
  /* Turn left when the robot sees black on the right sensor */
  } else if (pinL == 0 && pinR >= 1) {
    turnLeft(50);

  /* Turn right when the robot sees black on the left sensor */
  } else if (pinL >= 1 && pinR == 0) {
    turnRight(50);
    
  /* When the robot doesn't see black on either of the sensors, drive forward */
  } else if (pinL == 0 && pinR == 0 && drive) {
    driveForward(40);
  }
}

/* Drive Forward */
void driveForward(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}

/* Turn left */
void turnLeft(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
}

/* Turn right */
void turnRight(double percentage)  {
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL + 5);
}

/* Stand still */
void standStill() {
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
}

/* Drive backwards */
void driveBackwards(){
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
}
