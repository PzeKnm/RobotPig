#include <Arduino.h>

#include "Movements.h"
#include "MoreOrLessProtocol.h"

#define SERIAL_BAUD 115200

int ledPin = 13;

CMovements m_movements;

#define SLAVE_ADDR_ROBOTPIG 0x2d
CMoreOrLessProtocol m_protocol;

#define COMMAND_BASED false

unsigned long m_Millis;

// Right now, the only way to determine if the answer was correct is to compare the 
// old score with the incoming data.
int nOldScore = 0;

// And we need to know when we're transitioning into the "question answered" state
bool bInAnsweredState = false;

// #define NUM_COMMANDS 10

// S01 rotation goes from 0 to 150, midpoint 75

// S02 30 raised, 100 lowered

// S03 160 forward, 30 back

// S04P080 - jaw open
// S04P030 - jaw shut

// S01 - Nod Forward 170 - back 20     mid 110
// S02 - Left Arm Up - 40 down - 160   mid 120
// S03 - Right Arm  up 80 - down 10    mid  40
// S04 - shake left 180 right 0        mid 100

/*
#define POT_1   A1
#define POT_2   A2
#define POT_3   A3
#define POT_4   A4

#define DI_RUN_ANIMATION 2
#define DI_ANIMATION_TYPE 3
#define DI_ANIMATION_MSB 4
#define DI_ANIMATION_LSB 5
*/
 #define DO_SERVO_POWER 7

 
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




/////////////////////////////////////////////////////////////////////////////////////// 

  enum Movement_Type {
    EyesFront = 0,
    Nod = 1,
    Shake = 2,
    WaveLeft = 3,  
    WaveRight = 3,        
  };

// S1: Nod
// S2: Shake
// S3: Left arm
// S4: Right arm

// Command 0 . Eyes front, rest position
const char* const m_cmd0 PROGMEM =
{
    "\
S1P090,S2P090,S3P090,S4P100:1000ms"
};

//-----------------------------------------------------------------------------------
// nod of the head 
const char* const m_cmd1 PROGMEM =
{
  "\
S1P110,S2P090,S3P035,S4P100:0500ms\
S1P070,S2P090,S3P035,S4P100:0300ms\
S1P110,S2P090,S3P035,S4P100:0500ms\
S1P070,S2P090,S3P035,S4P100:0300ms\
S1P090,S2P090,S3P035,S4P100:0500ms"
};

//-----------------------------------------------------------------------------------
// shake of the head 
const char* const m_cmd2 PROGMEM =
{
  "\
S1P090,S2P090,S3P035,S4P100:0500ms\
S1P090,S2P130,S3P035,S4P100:0300ms\
S1P090,S2P050,S3P035,S4P100:0500ms\
S1P090,S2P130,S3P035,S4P100:0300ms\
S1P090,S2P090,S3P035,S4P100:0500ms"
};

//-----------------------------------------------------------------------------------
// wave left arm
const char* const m_cmd3 PROGMEM =
{
  "\
S1P090,S2P090,S3P090,S4P100:0500ms\
S1P090,S2P090,S3P140,S4P100:0500ms\
S1P090,S2P090,S3P090,S4P100:0500ms"
};

//-----------------------------------------------------------------------------------
// wave right arm
const char* const m_cmd4 PROGMEM =
{
  "\
S1P090,S2P090,S3P090,S4P100:0500ms\
S1P090,S2P090,S3P090,S4P050:0500ms\
S1P090,S2P090,S3P090,S4P100:0500ms"
};

/*
const char* const m_cmd3 PROGMEM =
{
  "\
S1P090,S2P130,S3P035,S4P100:0500ms\
S1P090,S2P090,S3P035,S4P100:0300ms\
S1P090,S2P130,S3P035,S4P100:0500ms\
S1P090,S2P170,S3P035,S4P100:0300ms\
S1P090,S2P130,S3P035,S4P100:0500ms"
};*/
/////////////////////////////////////////////////////////////////////////////////////// 


void SetupCommandSets()
{
  m_movements.SetCommandSet(EyesFront, m_cmd0);
  m_movements.SetCommandSet(Nod, m_cmd1);
  m_movements.SetCommandSet(Shake, m_cmd2); 
  m_movements.SetCommandSet(WaveLeft, m_cmd3);   
  m_movements.SetCommandSet(WaveRight, m_cmd4);  

}




/*
void IssueNextCommand()
 {
    m_ixCurrentCmd++;
    if(m_ixCurrentCmd > (NUM_COMMANDS - 1))
      m_ixCurrentCmd = 0;

    char* pszCmd = m_cmds[m_ixCurrentCmd];
    m_set.ApplyCommands(pszCmd);
 }

void DoCommandBased()
{
  int nElaspedMs = millis() - m_Millis;
  m_Millis = millis();

  if(m_set.ProcessTime(nElaspedMs) == false)
  {
    IssueNextCommand();
  }


  delay(10);
}*/

/*
int GetMappedAnalogValue(int nPin)
{
  int nCount = 10;
  int sum = 0;
  for(int n = 0; n < nCount; n++)
  {
    sum += analogRead(nPin); 
  }

  int val = sum / nCount; 
  int nDeg = map(val, 0, 1023, 0, 180);
  return nDeg;
}*/

/*
void DoAnalogBased()
{
  bool bSomethingMoved = false;

  int nDeg1 = GetMappedAnalogValue(POT_1);
  if(m_set.SetServoPos(1, nDeg1) == true)
    bSomethingMoved = true;

  int nDeg2 = GetMappedAnalogValue(POT_2);
  if(m_set.SetServoPos(2, nDeg2) == true)
    bSomethingMoved = true;

  int nDeg3 = GetMappedAnalogValue(POT_3);
  if(m_set.SetServoPos(3, nDeg3) == true)
    bSomethingMoved = true;

  int nDeg4 = GetMappedAnalogValue(POT_4);
  if(m_set.SetServoPos(4, nDeg4) == true)
    bSomethingMoved = true;

  if(bSomethingMoved)
  {  
    Serial.print("NewPos: ");
    Serial.print(nDeg1);
    Serial.print("\t");
    Serial.print(nDeg2);
    Serial.print("\t");
    Serial.print(nDeg3);
    Serial.print("\t");
    Serial.println(nDeg4);
  }
}
*/



/*
void SetServoPositions(int nPos1, int nPos2, int nPos3, int nPos4, int ms)
{
  if(nPos1 >= 0)
    m_set.ApplyCommandToServo(1, nPos1, ms);
  if(nPos2 >= 0)
    m_set.ApplyCommandToServo(2, nPos2, ms);
  if(nPos3 >= 0)
    m_set.ApplyCommandToServo(3, nPos3, ms);
  if(nPos4 >= 0)
    m_set.ApplyCommandToServo(4, nPos4, ms);

  m_set.ProcessTimeUntilServoAtPosition();
}


// S01 - Nod        Forward 50 - back 160     mid 90
// S02 - Left Arm    Up -   160   down - 10   mid 70
// S03 - Right Arm   up     10 -   down 170    mid  90
// S04 - shake       left 180   right 0        mid 100

void GotoHeadUpArmsUp()
{
  SetServoPositions(90, 140, 20, 100, 1000);
}

void EyesFront()
{
  SetServoPositions(90, 70, 90, 100, 1000);
}

void NodHead()
{

  SetServoPositions(60, -1, -1, -1, 400);
  SetServoPositions(90, -1, -1, -1, 400);
  SetServoPositions(120,  -1, -1, -1, 400);
  SetServoPositions(90, -1, -1, -1, 400);
  SetServoPositions(60, -1, -1, -1, 400);
  SetServoPositions(90, -1, -1, -1, 400);
}

void QuickNodHead()
{

  SetServoPositions(60, -1, -1, -1, 200);
  SetServoPositions(100,  -1, -1, -1, 200);
  SetServoPositions(60, -1, -1, -1, 200);
  SetServoPositions(100,  -1, -1, -1, 200);  
  SetServoPositions(60, -1, -1, -1, 200);
  SetServoPositions(100,  -1, -1, -1, 200);  
  SetServoPositions(90, -1, -1, -1, 200);
}

void ShakeHead()
{
  int ms = 800;
  SetServoPositions(-1, -1, -1, 100, ms);
  SetServoPositions(-1, -1, -1, 140, ms);
  SetServoPositions(-1, -1, -1, 40, ms);
  SetServoPositions(-1, -1, -1, 140, ms);
  SetServoPositions(-1, -1, -1, 100, ms);
}

void ArmsUpAndDown()
{
  SetServoPositions(-1, 70, 90, -1, 600);
  SetServoPositions(-1, 140, 160, -1, 400);
  SetServoPositions(-1, 30, 20, -1, 600);
  SetServoPositions(-1, 140, 160, -1, 600);
  SetServoPositions(-1, 30, 20, -1, 600);
  SetServoPositions(-1, 140, 160, -1, 600);
  SetServoPositions(-1, 30, 20, -1, 600);
  SetServoPositions(-1, 140, 160, -1, 600);
  SetServoPositions(-1, 70, 90, -1, 300);
}
*/

/*
void DoSwitchBased()
{
  int val = 0;
  
  val = digitalRead(DI_RUN_ANIMATION);
  if(val == 1)
  {
  //  Serial.println("Don't Run animation");
    return;
  }

  int nType = digitalRead(DI_ANIMATION_TYPE);

  int nVariation = digitalRead(DI_ANIMATION_LSB) + (2 * digitalRead(DI_ANIMATION_MSB));
   Serial.println(nVariation); 
  if(nType == 1)
  {
    Serial.println("Yay!!!");

    QuickNodHead();
  }
  else
  {
    Serial.println("Doh!!!");

    ShakeHead();
  }

}*/


int GetMovementCommand(SVizData vd)
{
  // special test state
  if(vd.GameState == Initialised)
  {
    return WaveRight;
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
            return Nod;  // Supernod would be better 
          if(vd.PotentialScore > 6)
            return Nod;  // Still pretty good
          if(vd.PotentialScore > 4)
            return Nod;  // Meh, could have been a guess   
          if(vd.PotentialScore > 2)
            return Nod;  // Not very confident 

          // This is the monkey level
          return Nod;                              
        }
        else
        {
          if(vd.PotentialScore > 8)
            return Shake;  // A triumph of confidence over knowledge
          if(vd.PotentialScore > 6)
            return Shake;  // At least they're trying to play the game
          if(vd.PotentialScore > 4)
            return Shake;  // Not very ambitious  
          if(vd.PotentialScore > 2)
            return Shake;  // Do they understand how this is meant to work

          // This is the monkey level
          return Shake;           
        }
       
      }
    }
  }

  // if we're not in the answered state, set the bool back again
  if(bInAnsweredState == true && vd.InternalState != DisplayAnswer)
    bInAnsweredState = false;

  return EyesFront;
}




void setup() 
{ 
  // m_ixCurrentCmd = -1;
  Serial.begin(SERIAL_BAUD);
  delay(1000);
  while(!Serial) {} // Wait
  Serial.println(F("Program RoboPig begins."));


  pinMode(DO_SERVO_POWER, OUTPUT);
  digitalWrite(DO_SERVO_POWER, LOW);

  pinMode(ledPin, OUTPUT);  
  blink(3);
  // m_set.Init();

  m_protocol.Setup(SLAVE_ADDR_ROBOTPIG, true);

  m_movements.Init();
  SetupCommandSets();


  m_Millis = millis();

  m_movements.DoCommand(EyesFront);

  Serial.println(F("Power on."));
  digitalWrite(DO_SERVO_POWER, HIGH);

} 
  

void loop() 
{  
  
  if(m_protocol.HasVizDataArrived()/* || bDoOnce*/)
  {
    Serial.println(F("Data arrived"));

    SVizData vd = m_protocol.GetVizDataConsuming();

    int nCmd = GetMovementCommand(vd);
    if(nCmd > 0)
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

  delay(100);
/* 
DoAnalogBased();
  
  if(COMMAND_BASED)
    DoCommandBased();
  else
    DoAnalogBased();
*/


/*
  GotoHeadUpArmsUp();
  delay(5000);*/


//  DoSwitchBased();
 // delay(500);
  // EyesFront();
 /// delay(5000);



/*
  NodHead();
  delay(5000);

  EyesFront();
  delay(5000); 

  ShakeHead();
  delay(5000);

  ArmsUpAndDown();
  delay(5000); 

  QuickNodHead();
  delay(5000); 
*/

} 