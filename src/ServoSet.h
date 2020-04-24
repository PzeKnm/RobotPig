/* 
* CMjkBlink.h
*
* Created: 10/08/2015 17:13:58
* Author: PZEKNM
*/

#ifndef __CServoSet_H__
#define __CServoSet_H__


#include <ServoMotor.h> 



class CServoSet
{

public:
	CServoSet();
	~CServoSet();

  // Attaches motors to pins
  void Init(); 



  // starts moving the motors to the given position over given time.
  bool ApplyCommandToServo(int nServo, int pos, int ms);

  // Moves the servos to their required positions. Returns false if no more movements required, 
  // or true if it timedout
  bool ProcessTimeUntilServoAtPosition(int nMaxMs = 10000);

  // returns true if there's still movements to be made, else false to indicate ready for next command
  bool ProcessTime(int ms);

  void sweep(int ixServo);

  void QuickTurn(int ixServo);
  
  void SweepAll();

  bool SetServoPos(int nServo, int nPos);

  bool ApplyCommands(const char* pszCmd);

protected:

private:

  int GetCommandTime(const char* pszCmd);

  int GetIntFromCmdLine(const char* pszCmd, int ixStart, int length);
  

#define NUM_SERVOS 4


  ServoMotor myServos[NUM_SERVOS];

 
  int servo0Pin;
  int servo1Pin;
  int servo2Pin;
  int servo3Pin; 


  int nDelayUS;
  int nSweepStart;
  int nSweepEnd;  
  int nSweepStep;

  
}; 

#endif //__CServoSet_H__













