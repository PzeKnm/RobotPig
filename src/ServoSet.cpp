


#include "ServoSet.h"
#include "Arduino.h"

#define BATT_CHECK_DEFAULT_ACTIVATE_PIN     12
#define BATT_CHECK_DEFAULT_READING_PIN      33


CServoSet::CServoSet()
{  
  servo0Pin = 3;
  servo1Pin = 5;
  servo2Pin = 9;
  servo3Pin = 11; 


  nDelayUS = 2800;
  nSweepStart = 5;
  nSweepEnd = 175;  
  nSweepStep = 1;
} 


CServoSet::~CServoSet()
{

} 


void CServoSet::Init()
{
  myServos[0].AttachToPin(servo0Pin, 75);
  myServos[1].AttachToPin(servo1Pin, 90);  
  myServos[2].AttachToPin(servo2Pin, 90);  
  myServos[3].AttachToPin(servo3Pin, 90);   


}


  // Each command set has the format:
  //            1         2         3
  //   1234567890123456789012345678901234
  //  "S1P110,S2P140,S3P035,S4P100:1000ms"
bool CServoSet::ApplyCommands(const char* pszCmd)
{
  // Serial.println(pszCmd);

  int nCmdTime = GetCommandTime(pszCmd);

  for(int n = 0; n < NUM_SERVOS; n++)
  {
    int nPos = GetIntFromCmdLine(pszCmd, 3 + (n * 7) , 3);  
 /**/
    Serial.print("nServo: "); Serial.print(n); Serial.print(" nPos: "); Serial.println(nPos);

    myServos[n].SetNewTargetPosition(nPos, nCmdTime);
  }

  return true;
}


bool CServoSet::ApplyCommandToServo(int nServo, int pos, int ms)
{
  myServos[nServo - 1].SetNewTargetPosition(pos, ms);
  return true;
}


bool CServoSet::ProcessTimeUntilServoAtPosition(int nMaxMs)
{
  unsigned long current = millis();
  bool bStillMoving = true;
  bool bTimeout = false;

  int TotalElapsed = 0; 

  while(bStillMoving && !bTimeout)
  {
    int nElaspedMs = millis() - current;
    current = millis();     
    bStillMoving = ProcessTime(nElaspedMs);

    TotalElapsed += nElaspedMs;
    if(TotalElapsed > nMaxMs)
      bTimeout = true;

    delay(50);
  }

  return bTimeout;
}



int CServoSet::GetCommandTime(const char* pszCmd)
{
  // Each command set has the format:
  //            1         2         3
  //   1234567890123456789012345678901234
  //  "S1P110,S2P140,S3P035,S4P100:1000ms"
  int nTime = GetIntFromCmdLine(pszCmd, 28, 4);
  if(nTime > 0)
    return nTime;
  return 1;
}


int CServoSet::GetIntFromCmdLine(const char* pszCmd, int ixStart, int length)
{
  char subbuff[length + 1];
  memcpy( subbuff, &pszCmd[ixStart], length );
  subbuff[length] = '\0';
  int nValue;
  sscanf(subbuff, "%d", &nValue);
  return nValue;
}




bool CServoSet::ProcessTime(int ms)
{
  bool bWorkRemaining = false;
  for(int f = 0; f < NUM_SERVOS; f++)
  {
    if(myServos[f].ProcessTime(ms))
      bWorkRemaining = true;
  }

  return bWorkRemaining;
}

bool CServoSet::SetServoPos(int nServo, int nPos)
{
  return myServos[nServo - 1].SetPosition(nPos);
}


void CServoSet::SweepAll()
{
  /* 
  for (int pos = nSweepStart; pos <= nSweepEnd; pos += nSweepStep) 
  {   
    myServos[0].write(pos); 
    myServos[1].write(pos); 
    myServos[2].write(pos);                     
    delayMicroseconds(nDelayUS * 4);
  }*/
}

void CServoSet::sweep(int ixServo)
{ 
  /* 
  int pos = 0;

  for (pos = nSweepStart; pos <= nSweepEnd; pos += nSweepStep) 
  {   
    myServos[ixServo].write(pos);                 
    delayMicroseconds(nDelayUS);
  }

  for (pos = nSweepEnd; pos >= nSweepStart; pos -= nSweepStep) 
  { 
    myServos[ixServo].write(pos);             
    delayMicroseconds(nDelayUS);                
  } */ 
}

void CServoSet::QuickTurn(int ixServo)
{
  /* 
  myServos[ixServo].write(nSweepStart);
  delay(500);
  myServos[ixServo].write(nSweepEnd); 
  delay(500);   
  myServos[ixServo].write(nSweepStart);  */
}
 



  