#include<MobaTools.h>
const byte stepPin =18;
const byte dirPin =17;

#define limitPin A11
#define startPin 14
#define collectPin 9
const byte senPin=A10;

const int stepsperRev = 300;


long  nextPos;
const long targetPos =3400;

int count = 0;
int limit_count =0;

bool limswitch =0;

int hOme =0;
int pos =0;
int sTart =0;
int cloth = 0;

int cloth_count = 0;

MoToStepper stepper1(stepsperRev, STEPDIR);
MoToTimer stepperPause;

bool stepperRunning;
bool movingForward;
bool clothDetected;

void setup() {
  Serial.begin(9600);
  stepper1.attach(stepPin,dirPin);
  stepper1.setRampLen(10);
  stepperRunning = false;
  movingForward = true;
  clothDetected = false;

  pinMode(limitPin, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(senPin, INPUT_PULLUP);
  pinMode(collectPin, INPUT_PULLUP);
  homing();
  set_pos();
}

void loop() {
  int c =digitalRead(collectPin);
    while(digitalRead(startPin)==LOW)
    {sTart =1;}

    
    if((hOme ==2)&& (limit_count>1)&&(count ==0)&&(pos=1)&& (sTart ==1)) {
    int sen = digitalRead(senPin);
    bool sensorFound = false;

    if(sen==0 || sen==1){
      sensorFound = true;
    }

    if(sen==1){
      clothDetected =false;
      motor();
      Serial.print("CLOTHCOUNT:");Serial.println(cloth_count);}
      }
    if(c==0) {
      collect();
      homing();
      set_pos();
    }
}

void homing() {
  stepper1.setSpeed(1500);
  while(digitalRead(limitPin) != HIGH && limit_count==0) { stepper1.rotate(-1);Serial.print(limswitch);Serial.println("Finding END");}

  if(digitalRead(limitPin) == HIGH && limit_count ==0) {stepper1.stop();Serial.println("END POSITION");limit_count+=1;limswitch=HIGH;hOme =1;}
  stepper1.setZero();
  delay(2000);
  stepper1.setSpeed(80);stepper1.moveTo(500);
  limswitch =LOW;
  delay(2000);
  while(digitalRead(limitPin) != HIGH && limit_count==1) { stepper1.rotate(-1);Serial.print(limswitch);Serial.println("Finding END2");}
  if(digitalRead(limitPin) == HIGH && limit_count ==1) {stepper1.stop();Serial.println("END POSITION2");limit_count+=1;limswitch=HIGH;hOme =2;}
  Serial.print("limit_count:");Serial.println(limit_count);
  Serial.print("limswitch:");Serial.println(limswitch);
  Serial.print("hOme:");Serial.println(hOme);
  Serial.print("POS:");Serial.println(pos);
  stepper1.setZero();
  delay(2000);
  }

void set_pos() {
  
  while(hOme ==2 && limit_count >1 && limswitch ==HIGH && pos ==0) {
    
    delay(1000);
    stepper1.setSpeed(1300);
    stepper1.moveTo(4000);

    while(stepper1.moving()){Serial.println("MOVING to Pos1");}

    pos =1;
    Serial.println("StartPositionSet, READY TO USE");
    }
    Serial.print("POS:");Serial.println(pos);
}


void motor() {

  if (!stepperRunning && !clothDetected)
  {
    // Set flag to true when signature is not detected
    clothDetected = true;

    //MoveForward
    stepper1.setSpeed(1200);
    stepper1.move(-targetPos);
    Serial.println("Moving Forward");
    cloth_count++;
    stepperRunning = true;

    while(stepper1.moving())
    {
      delay(10);//wait untill one half oscillation
    }


    //Move Backward
    stepper1.setSpeed(3500);
    stepper1.move(targetPos);
    Serial.println("Moving BAck");

    while(stepper1.moving()) {
      delay(10);
    }

    stepperRunning = false;
  }
}

void collect() {
  stepper1.setSpeed(1500);
  stepper1.moveTo(targetPos);
  stepper1.stop();
  stepperPause.setTime(5000);
  cloth_count = 0;
  sTart =0;
  limit_count = 0;
  hOme = 0;
  pos = 0;
}
