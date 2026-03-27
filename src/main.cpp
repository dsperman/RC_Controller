#include <Arduino.h>

void RunMotor(int, int, int);
int RunToHigh(int, int, int);
int RunToLow(int, int, int);
int InitMotor();


const int stepPin = 8; // Step otuput
const int dirPin = 9;  // Direction output
const int enaPin = 10;  // Enable output
const int proxPin = 6;  //Proximity switch input
int CycleSteps;
int StepDelay;
int StepCount;
int HalfDwell;
bool RunOnce;  

void setup()
{
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enaPin, OUTPUT);
    pinMode(proxPin, INPUT);
    digitalWrite(enaPin, HIGH);
    delay(500);
    Serial.begin(9600);
    StepDelay=500;          
    CycleSteps=1000;
    CycleSteps=InitMotor();
    
}

void loop()
{

  char WriteVal[64];   


      Serial.write("CycleSteps: ");
      sprintf(WriteVal, "%d",CycleSteps);
       Serial.write(WriteVal);
      Serial.write("\r\n");
      
    
    RunMotor(1, CycleSteps, StepDelay);

   delay(1000);

    

  }


void RunMotor(int Direction, int Steps, int Speed)
//void RunMotor(int Direction, int Speed, int Steps)
{
    digitalWrite(dirPin, Direction); // Set direction 
    digitalWrite(enaPin, HIGH);
    Serial.write("Run Motor \r\n");
  
    for (int i = 0; i < Steps; i++)
    { 
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(2); // Adjust for speed
      digitalWrite(stepPin, LOW);
      delayMicroseconds(Speed); // Adjust for speed
    }
}



int InitMotor()
{
    int DwellSteps;
    int TotalSteps;
    long Edges[9];
    int EdgeState[9];
    int EdgeCount;
    bool LastProx=false;
    bool CrntProx=false;
    char WriteVal[64];

    EdgeCount=0;

   
    digitalWrite(dirPin, 1); // Set direction 
    //return 0;

    LastProx=digitalRead(proxPin);

    for (long i = 0; i < 1000000; i++)
    { // Travel 3200 Steps
      
      CrntProx=digitalRead(proxPin);
      
      if (LastProx != CrntProx)
        {
        sprintf(WriteVal, "%d",EdgeCount);
        Serial.write(WriteVal);
        Serial.write(": ");
        
        sprintf(WriteVal, "%d",i);
        Serial.write(WriteVal);
        Serial.write(": ");
       
        sprintf(WriteVal, "%d", CrntProx);
        Serial.write(WriteVal);
        Serial.write("\r\n");
   
        Edges[EdgeCount] = i;
        EdgeState[EdgeCount]=CrntProx;

        
        EdgeCount++;  
                
        if (EdgeCount > 9)
          {
            i=1000001;
          }
        }
        LastProx=CrntProx;
      
  
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(2); // Adjust for speed
        digitalWrite(stepPin, LOW);
        delayMicroseconds(200); // Adjust for speed
    }  
     


    if (EdgeState[2]==0)
    {
      Serial.write("Running If \r\n");
  
      TotalSteps=(((Edges[3]-Edges[1])+(Edges[5]-Edges[3])+(Edges[7]-Edges[5]))/3);
      
      DwellSteps=(((Edges[3]-Edges[2])+(Edges[5]-Edges[4])+(Edges[7]-Edges[6]))/3);

      HalfDwell=(DwellSteps/2);
      delay(100);
      RunMotor(0,(HalfDwell),150);
    delay(1000);
 
    }
    else
    {
      Serial.write("Running Else \r\n");
      TotalSteps=(((Edges[4]-Edges[2])+(Edges[6]-Edges[4])+(Edges[8]-Edges[6]))/3);
      
      DwellSteps=(((Edges[2]-Edges[1])+(Edges[4]-Edges[3])+(Edges[6]-Edges[5]))/3);

      HalfDwell=(DwellSteps/2);
      delay(100);
      RunMotor(1,(HalfDwell),150);
    delay(1000);
      

    }
    HalfDwell=(DwellSteps/2);
    

    
     sprintf(WriteVal, "%d",HalfDwell);
     Serial.write("Half Dwell: ");
     Serial.write(WriteVal);
     Serial.write("\r\n");
     
     sprintf(WriteVal, "%d",TotalSteps);
 
     Serial.write("Total Steps: ");
     Serial.write(WriteVal);
     Serial.write("\r\n");
 
    delay(1000);
    return TotalSteps;
 

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
    return 0;
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
    return 0;
}