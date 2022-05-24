#include <Arduino.h>
#include "NexButton.h"

#define MOTOR_STEPS 400
#define RPM 160

#define DIR 4
#define STEP 7

#include "DRV8825.h"
DRV8825 stepper(MOTOR_STEPS, DIR, STEP);

int nasaj = 11;
int vysaj = 12;
int vyhod = 13;
int nasate = 3;
int vysate = 6;
int vyhodene = 9;


const int endstop = A7; // pin that the sensor is attached to
const int threshold = 400; // an arbitrary threshold level that's in the range of the analog input
const int ledPin = 13;
const int tkroku = 2000;

static int stepsDone = 0;
static int totalSteps = 0;
static int rotation = 0;

NexButton dvestoul = NexButton(1, 1, "b0");
NexButton tristoul = NexButton(1, 2, "b1");
NexButton calibrate = NexButton(1, 3, "b2");
NexButton servis= NexButton(1, 4, "b3");

// Register a button object to the touch event list.  
NexButton *nex_listen_list[] = {
  &dvestoul,
  &tristoul,
  &calibrate,
  &servis,
  NULL
};

void dvestoulPushed(void *ptr) {
  // vydrbat to do pice serial printy
  Serial.print("dvestoulPushed, rotation set to: ");
  rotation = 4300;
  Serial.println(rotation);
  }


void tristoulPushed(void *ptr) {
  // vydrbat to do pice serial printy
  Serial.print("tristoulPushed, rotation set to: ");
  rotation = 7300;
  Serial.println(rotation);
  }

void calibratePushed(void *ptr) {
  if (endstop < threshold){
    stepper.rotate(3600);
    digitalWrite(ledPin, LOW);
  }
  if (endstop > threshold){
    digitalWrite(ledPin, HIGH);
    stepper.rotate(-750);
  }
}

void servisPushed(void *ptr) {
  stepper.rotate(-10000);
}


void setup() {
  dvestoul.attachPush(dvestoulPushed);
  tristoul.attachPush(tristoulPushed);
  calibrate.attachPush(calibratePushed);
  servis.attachPush(servisPushed);
  Serial.begin(9600); 
    stepper.begin(RPM);
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
    stepper.setEnableActiveState(LOW);
    stepper.enable();
    stepper.setMicrostep(1);  // Set microstep mode to 1:1
    pinMode(ledPin, OUTPUT);
    pinMode(endstop, INPUT);
    pinMode(nasaj, INPUT);
    pinMode(vysaj, INPUT);  
    pinMode(vyhod, INPUT);
    pinMode(nasate, OUTPUT);
    pinMode(vysate,  OUTPUT);
    pinMode(vyhodene, OUTPUT);
//    pinMode(objem, OUTPUT);
    
}

void loop() {
  nexLoop(nex_listen_list);
  if(digitalRead(nasaj) == HIGH){
    stepper.rotate(-200);
    stepper.rotate(-rotation);
    digitalWrite(nasate,HIGH);
    stepper.rotate(-400);
    delay(2000);
    digitalWrite(nasate, LOW);
    }

  if(digitalRead(vysaj) == HIGH){
    stepper.rotate(rotation);
    stepper.rotate(200);
    digitalWrite(vysate,HIGH);
    delay(200);
    digitalWrite(vysate, LOW);
  }

  if(digitalRead(vyhod) == HIGH){
    stepper.rotate(720);
    delay(200);
    stepper.rotate(-720);
    digitalWrite(vyhodene,LOW);
    delay(200);
    digitalWrite(vyhodene, LOW);
     } 
}
