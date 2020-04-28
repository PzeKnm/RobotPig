#include <Arduino.h>

#include "Movements.h"
#include "MoreOrLessProtocol.h"

#define SERIAL_BAUD 115200

int ledPin = 13;

CMovements m_movements;

#define SLAVE_ADDR_ROBOTPIG 0x2d
CMoreOrLessProtocol m_protocol;


unsigned long m_Millis;

// Right now, the only way to determine if the answer was correct is to compare the 
// old score with the incoming data.
int nOldScore = 0;

// And we need to know when we're transitioning into the "question answered" state
bool bInAnsweredState = false;

#define DO_SERVO_POWER 7

 



/////////////////////////////////////////////////////////////////////////////////////// 


int GetMovementCommand(SVizData vd)
{
  // special test state
  if(vd.GameState == Initialised)
  {
    if(vd.PotentialScore < NUMBER_OF_MOVEMENTS)
      return vd.PotentialScore;
    else
      return (NUMBER_OF_MOVEMENTS - 1);      
  }

  // During game play, the pig should indicate correct and incorrect answers
  if(vd.GameState == GamePlaying)
  {
    if(vd.InternalState == DisplayAnswer)
    {
      if(bInAnsweredState == false)
      {
        // First transition to answered state
        bInAnsweredState = true;
        //
        int nScoreChange = vd.Score - nOldScore;
        // based upon how big the change was, do a more energetic victory dance
        // and if the range was anyway modest, and the answer incorrect, then 
        // do a more insulting failure motion.
        nOldScore = vd.Score;
        if(nScoreChange > 0)
        {
          if(vd.PotentialScore > 8)
            return CMovements::ThankYouLord;  // Supernod would be better 
          if(vd.PotentialScore > 6)
            return CMovements::PirateDance;  // Still pretty good
          if(vd.PotentialScore > 4)
            return CMovements::Hooray;  // Meh, could have been a guess   
          if(vd.PotentialScore > 2)
            return CMovements::Nod;  // Not very confident 

          // This is the monkey level
          return CMovements::Nod;                              
        }
        else
        {
          if(vd.PotentialScore > 8)
            return CMovements::QuickShake;  // A triumph of confidence over knowledge
          if(vd.PotentialScore > 6)
            return CMovements::QuickShake;  // At least they're trying to play the game
          if(vd.PotentialScore > 4)
            return CMovements::OhhhNoooo;  // Not very ambitious  
          if(vd.PotentialScore > 2)
            return CMovements::OhhhNoooo;  // Do they understand how this is meant to work ??!?

          // This is the monkey level
          return CMovements::ShakeHead;           
        }
       
      }
    }
  }

  // if we're not in the answered state, set the bool back again
  if(bInAnsweredState == true && vd.InternalState != DisplayAnswer)
    bInAnsweredState = false;

  return CMovements::EyesFront;
}


void blink(int nTimes)
{
  for(int n = 0; n < nTimes; n++)
  {
    digitalWrite(ledPin, HIGH);  
    delay(200);             
    digitalWrite(ledPin, LOW);    
    delay(200);              
  }
}

void setup() 
{ 
  Serial.begin(SERIAL_BAUD);
  delay(1000);
  while(!Serial) {} // Wait
  Serial.println(F("Program RoboPig begins."));

  pinMode(DO_SERVO_POWER, OUTPUT);
  digitalWrite(DO_SERVO_POWER, LOW);

  pinMode(ledPin, OUTPUT);  
  blink(3);

  m_protocol.Setup(SLAVE_ADDR_ROBOTPIG, true);

  m_movements.Init();

  m_Millis = millis();

  m_movements.DoCommand(CMovements::EyesFront);

  Serial.println(F("Power on."));
  digitalWrite(DO_SERVO_POWER, HIGH);
} 
  
void loop() 
{    
  if(m_protocol.HasVizDataArrived())
  {
    Serial.println(F("Data arrived"));

    SVizData vd = m_protocol.GetVizDataConsuming();

    int nCmd = GetMovementCommand(vd);
    if(nCmd >= 0)
    {
      if(!m_movements.IsBusy())
      {
        // evaluate what kind of movement is required and commence it.
        m_movements.DoCommand(nCmd);
      }
      else
      {
        // pig currently occupied doing something. store cmd for later?
        m_movements.CueUpNextCommand(nCmd);
      }       
    }  
  }

  // Process pig movements. Or should that be "Porcess"??!
  int nElaspedMs = millis() - m_Millis;
  m_Millis = millis();

  if(m_movements.ProcessTime(nElaspedMs) == false)
  {
    // If there are any more commands left, move on to next one.
    ///  IssueNextCommand();
  }

  delay(20);
} 