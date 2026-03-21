#include <Arduino.h>

void RunMotor(int, int, int);
int RunToHigh(int, int, int);
int RunToLow(int, int, int);
int InitMotor();


#include "Arduino.h"
const int stepPin = 8; // Step otuput
const int dirPin = 9;  // Direction output
const int enaPin = 10;  // Enable output
const int proxPin = 6;  //Proximity switch input
int StepsPerRev;
int MicroSteps;
int CycleSteps;
int StepDelay;
int StepCount;
  

void setup()
{
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enaPin, OUTPUT);
    pinMode(proxPin, INPUT);
    digitalWrite(enaPin, HIGH);
    delay(500);
    StepDelay=500;          

    CycleSteps=InitMotor();



}

void loop()
{
 // StepCount =  RunToRise(1, TotalSteps, StepDelay);

    RunMotor(1, CycleSteps, StepDelay);

   delay(1000);


  }


void RunMotor(int Direction, int Steps, int Speed)
//void RunMotor(int Direction, int Speed, int Steps)
{
    digitalWrite(dirPin, Direction); // Set direction 

    delay(1000);               // wait 1s  
    for (int i = 0; i < Steps; i++)
    { 
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(2); // Adjust for speed
      digitalWrite(stepPin, LOW);
      delayMicroseconds(Speed); // Adjust for speed
    }
}

int RunToLow(int Direction, int Steps, int Speed)
// Run until prox switch goes Low
{
   bool Triggered;
     
    digitalWrite(dirPin, Direction); // Set direction 
    //return 0;

    for (int i = 0; i < Steps; i++)
    { // Travel 3200 Steps
        
  Triggered=digitalRead(proxPin);  //
       
      if (Triggered == LOW) {
          
          return i;
          i=Steps+1;
          
      }
  
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(2); // Adjust for speed
        digitalWrite(stepPin, LOW);
        delayMicroseconds(Speed); // Adjust for speed
        
  //    }
    }
}

int RunToHigh(int Direction, int Steps, int Speed)
// Run until prox switch goes High
{
   bool Triggered;
     
    digitalWrite(dirPin, Direction); // Set direction 
    //return 0;

    for (int i = 0; i < Steps; i++)
    { // Travel 3200 Steps
        
      Triggered=digitalRead(proxPin);  //
       
      if (Triggered == HIGH) {
          
          return i;
          i=Steps+1;
          
      }
  
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(2); // Adjust for speed
        digitalWrite(stepPin, LOW);
        delayMicroseconds(Speed); // Adjust for speed
        
  //    }
    }
}

int InitMotor()
{
    int DwellSteps;
    int NonDwellSteps;
    int TotalSteps;

    //get to start position
    NonDwellSteps=RunToLow(1, 10000, 500);
       delay(100);
     DwellSteps=RunToHigh(1, 10000, 500);
 
    delay(100);
    NonDwellSteps=RunToLow(1, 10000, 500);

    delay(100);
    DwellSteps=RunToHigh(1, 10000, 500);
    delay(100);
    NonDwellSteps=RunToLow(1, 10000, 500);
    delay(100);
  
    RunMotor(1,(DwellSteps/2), 500);  
    TotalSteps=DwellSteps+NonDwellSteps;
    return TotalSteps;



}