/* 
* CMjkBlink.h
*
* Created: 10/08/2015 17:13:58
* Author: PZEKNM
*/

#ifndef __CServoMotor_H__
#define __CServoMotor_H__


#include <Servo.h> 


class ServoMotor
{


public:

  ServoMotor();
	~ServoMotor();

  // Attaches servo to pin
  void AttachToPin(int nPin, int nInitialPos);

  // Sets the new position, and how long it should take to get there
  void SetNewTargetPosition(int nPos, int msMovementDuration);

  // Returns true if there are still movements to be made
  bool ProcessTime(int ms);

  // return true if moved
  bool SetPosition(int nDeg);

private:

  Servo m_servo;

  int m_nPin;

  // Thw position the servo is in before the move
  double m_StartPosition;

  // To what position the servo should drive
  double m_IntendedPosition;

  // How long it should take to get there
  int m_MovementDuration;

  // where it is right now
  double m_CurrentPosition;


};


#endif //__CServoMotor_H__













