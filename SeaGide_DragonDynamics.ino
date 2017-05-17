//Library Calls
#include <Servo.h>
#include <IRremote.h>

boolean pauseToggle = false;

boolean homeingRequired = false;


static byte servoRetractCommand = 0; //retract
static byte servoExtendCommand = 180; //extend
int riseDriveTime = 7750; //DO NOT GO OVER 15500!! THIS WILL CAUSE THE SERVO TO PUSH THE PLUNGER "THROUGH" THE BOYANCY ENGINE

float upperDepth = 0; //in inches (72 = 6 feet)
float lowerDepth = 12; //in inches (72 = 6 feet)

//Pins
static byte ENGINE_PIN = 10; //Continuious rotation servo
static byte LIMIT_SWITCH = 11;
static byte R_LED = 9;
static byte G_LED = 6;
static byte B_LED = 5;
static byte LED_BASE = 7; //4 pin led
int PRESSURE_SENSOR = A2;

static byte RECV_PIN = 2;            // IR reciever signal pin
static byte IR_GND = 3;              // IR middle pin, ground
static byte IR_PWR = 4;              // IR power pin
//

// IR definitions
//IRrecv irrecv(RECV_PIN);
//decode_results results;
IRrecv irrecv(RECV_PIN);
decode_results results;
#define PAUSE 0xFD807F 
#define ONE 0xFD08F7
#define TWO 0xFD8877
#define THREE 0xFD48B7
#define UP 0xFDA05F
#define DOWN 0xFDB04F
#define LEFT 0xFD10EF
#define RIGHT 0xFD50AF

//Object Declarations
Servo engineMotor;
//

void setup() {
  // put your setup code here, to run once:
  IRsetup();
  pinMode(LIMIT_SWITCH, INPUT_PULLUP);
  pinMode(R_LED, OUTPUT);          // initialise the RED
  pinMode(G_LED, OUTPUT);        //                GREEN
  pinMode(B_LED, OUTPUT);         //            and BLUE pins on the LED
  pinMode(LED_BASE, OUTPUT);         // initialize the common pin of the LED 
  digitalWrite(LED_BASE, HIGH);
  ledRGB_Write(0,0,0);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  setMotorPosition("retracted");

  while((getDepth() < lowerDepth) || paused()) {
    //
    if (paused()) {
      Serial.print("==PAUSED==");
      if (checkCenter()) {
        Serial.print("Centering...");
        setMotorPosition("retracted");
        setMotorPosition("center");
        homeingRequired = true;
        
      }
      if (checkBurb()) {
        ledRGB_Write(255,255,255);
        setMotorPosition("retracted");
        setMotorPosition("extended");
        setMotorPosition("retracted");
        setMotorPosition("extended");
        setMotorPosition("retracted");
        setMotorPosition("extended");
        homeingRequired = true;
      }
      ledRGB_Write(255,0,0);
    } else {
      ledRGB_Write(0,255,0);
      if (homeingRequired) {
        homeingRequired = false;
        setMotorPosition("retracted");
      }
    }


    
    Serial.print("depth: ");
    Serial.print(getDepth());
    Serial.print("    raw: ");
    Serial.println(analogRead(PRESSURE_SENSOR));
  }

  if (homeingRequired) {
    homeingRequired = false;
    setMotorPosition("retracted");
  }

  setMotorPosition("extended");

  while(getDepth() > upperDepth || paused()) {
    //
    if (paused()) {
      Serial.print("==PAUSED==");
      if (checkCenter()) {
        Serial.print("Centering...");
        setMotorPosition("retracted");
        setMotorPosition("center");
        homeingRequired = true;
        
      }

      if (checkBurb()) {
        ledRGB_Write(255,255,255);
        setMotorPosition("retracted");
        setMotorPosition("extended");
        setMotorPosition("retracted");
        setMotorPosition("extended");
        setMotorPosition("retracted");
        setMotorPosition("extended");
        homeingRequired = true;
      }
      ledRGB_Write(255,0,0);
    } else {
      ledRGB_Write(0,0,255);
      if (homeingRequired) {
        homeingRequired = false;
        setMotorPosition("retracted");
        setMotorPosition("extended");
      }
    }
    Serial.print("depth: ");
    Serial.print(getDepth());
    Serial.print("    raw: ");
    Serial.println(analogRead(PRESSURE_SENSOR));
  }

  if (homeingRequired) {
    homeingRequired = false;
    setMotorPosition("retracted");
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
  } else if (destination == "retracted") {
      //
      engineMotor.attach(ENGINE_PIN);
      engineMotor.write(servoRetractCommand);
      while(digitalRead(11) == HIGH) {
        //
        Serial.println("Waiting");
        engineMotor.write(servoRetractCommand);
      }
      engineMotor.detach();
    } else if (destination == "center") {
      engineMotor.attach(ENGINE_PIN);
      engineMotor.write(servoExtendCommand);

      delay(riseDriveTime / 2);

      engineMotor.detach();
    }
}



float getDepth() {
  //
  int raw = analogRead(PRESSURE_SENSOR);
  return (float) (1.59625 * raw) - 380.975;
}










void IRsetup(){
  irrecv.enableIRIn();
  pinMode(IR_GND, OUTPUT);
  pinMode(IR_PWR, OUTPUT);
  digitalWrite(IR_GND, 0);
  digitalWrite(IR_PWR, 1);
}





boolean checkPause(){
  if (irrecv.decode(&results)) {
    if (results.value == PAUSE) {
      Serial.println("PAUSE");
      irrecv.resume();
      return true;
    }
    else {
        irrecv.resume();
        return false;
      }
    }
  }

boolean checkCenter(){
  if (irrecv.decode(&results)) {
    if (results.value == TWO) {
      Serial.println("TWO");
      irrecv.resume();
      return true;
    }
    else{
      irrecv.resume();
      return false;
    }
  }
}

boolean checkBurb() {
  if (irrecv.decode(&results)) {
    if (results.value == ONE) {
      irrecv.resume();
      return true;
    }
    else{
      irrecv.resume();
      return false;
    }
  }
}

boolean paused() {
  if (checkPause()) {
    pauseToggle = !pauseToggle;
  }
  return pauseToggle;
}



void ledRGB_Write(byte R, byte G, byte B){      // This method takes care of the details of setting a color and intensity of the RGB LED
  analogWrite(R_LED, 255-R);                  // These are backwards because you write low values to turn these LEDs on
  analogWrite(G_LED, 255-G);                // This method reverses the counterintuitive nature of the LEDs
  analogWrite(B_LED, 255-B);                 // If using common anode rather than common anode LEDs remove the "255-"es
}  

