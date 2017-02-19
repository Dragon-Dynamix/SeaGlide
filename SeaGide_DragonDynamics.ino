//Library Calls
#include <Servo.h>

//


static byte servoRetractCommand = 0; //retract
static byte servoExtendCommand = 180; //extend
int riseDriveTime = 15500; //DO NOT GO OVER 15500!! THIS WILL CAUSE THE SERVO TO PUSH THE PLUNGER "THROUGH" THE BOYANCY ENGINE

//Pins
static byte ENGINE_PIN = 10; //Continuious rotation servo
static byte LIMIT_SWITCH = 11;
static byte R_LED = 9;
static byte G_LED = 6;
static byte B_LED = 5;
static byte LED_BASE = 7;
//

//Object Declarations
Servo engineMotor;
//

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(digitalRead(LIMIT_SWITCH));
  Serial.println("Retracting");
  setMotorPosition("retracted");
  Serial.println("Done");
  Serial.println("extending");
  setMotorPosition("extended");
  Serial.println(")
}

void loop() {
  // put your main code here, to run repeatedly:

}

void setMotorPosition(String destination) {
  //
  if (destination == "extended") {
      //
unsigned long currentMillis = millis();
long previousMillis = currentMillis;
engineMotor.attach(ENGINE_PIN);
engineMotor.write(servoExtendCommand);
while (currentMillis - previousMillis < riseDriveTime) {
  currentMillis = millis();
  //wait
}
engineMotor.detach();
      
  } else {
    if (destination == "retracted") {
      //
      engineMotor.attach(ENGINE_PIN);
      engineMotor.write(servoRetractCommand);
      while(digitalRead(LIMIT_SWITCH) == LOW) {
        //
      Serial.println("Waiting");
      }
      engineMotor.detach();
    }
  }
}

