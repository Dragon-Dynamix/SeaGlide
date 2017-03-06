//Library Calls
#include <Servo.h>

//


static byte servoRetractCommand = 0; //retract
static byte servoExtendCommand = 180; //extend
int riseDriveTime = 15500; //DO NOT GO OVER 15500!! THIS WILL CAUSE THE SERVO TO PUSH THE PLUNGER "THROUGH" THE BOYANCY ENGINE
int lowerDepth = 6; //in inches (72 = 6 feet)
int upperDepth = 6; //in inches (72 = 6 feet)

//Pins
static byte ENGINE_PIN = 10; //Continuious rotation servo
static byte LIMIT_SWITCH = 11;
static byte R_LED = 9;
static byte G_LED = 6;
static byte B_LED = 5;
static byte LED_BASE = 7; //4 pin led
static byte PRESSURE_SENSOR = 2;
//

//Object Declarations
Servo engineMotor;
//

void setup() {
  // put your setup code here, to run once:
  pinMode(LIMIT_SWITCH, INPUT_PULLUP);
  
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  setMotorPosition("retracted");

  while(getDepth() < lowerDepth) {
    //
    Serial.println("depth: " + getDepth());
  }

  setMotorPosition("extended");

  while(getDepth() < upperDepth) {
    //
    Serial.println("depth: " + getDepth());
  }

}

void setMotorPosition(String destination) {
  //
  if (destination == "extended") {
      //rising
engineMotor.attach(ENGINE_PIN);
engineMotor.write(servoExtendCommand);

delay(riseDriveTime);

engineMotor.detach();
      //
  } else {
    if (destination == "retracted") {
      //
      engineMotor.attach(ENGINE_PIN);
      engineMotor.write(servoRetractCommand);
      while(digitalRead(11) == HIGH) {
        //
        Serial.println("Waiting");
        engineMotor.write(servoRetractCommand);
      }
      engineMotor.detach();
    }
  }
}



int getDepth() {
  //
  int raw = analogRead(PRESSURE_SENSOR);
  return (raw - 240) / 0.58602;
}
