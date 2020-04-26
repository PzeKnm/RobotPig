


#include "ServoSet.h"
#include "Arduino.h"

#define BATT_CHECK_DEFAULT_ACTIVATE_PIN     12
#define BATT_CHECK_DEFAULT_READING_PIN      33


ServoMotor::ServoMotor()
{
  m_IntendedPosition = -1.0;
  m_MovementDuration = 1;
  m_CurrentPosition = -1.0;
}

ServoMotor::~ServoMotor()
{
  
}


void ServoMotor::AttachToPin(int nPin, int nInitialPos)
{
  m_nPin = nPin;
  m_CurrentPosition = nInitialPos;
  m_servo.write(m_CurrentPosition);
  m_servo.attach(m_nPin);
}

void ServoMotor::SetNewTargetPosition(int nPos, int msMovementDuration)
{
  if(nPos < 0)
    return;
  if(nPos > 180)
    return;

  m_StartPosition = m_CurrentPosition;
  m_IntendedPosition = nPos;
  m_MovementDuration = msMovementDuration;
}


bool ServoMotor::ProcessTime(int ms)
{
  // No position given as yet.
  if(m_IntendedPosition == -1.0)
    return false;

  if(m_CurrentPosition == m_IntendedPosition)
    return false;

  // timespan is enough to move to intended destination
  if(ms >= m_MovementDuration)
  {
    m_CurrentPosition = m_IntendedPosition;
    m_servo.write(m_CurrentPosition);

    PrintServoPos(m_nPin, m_CurrentPosition);
    return false;
  }

  // Move by proportionally how much time has elasped
  double dProportion = double(ms) / double(m_MovementDuration);
  double dDiff = m_IntendedPosition - m_StartPosition;

  // Amount to move might be a fraction of a click
  double dMove = double(dDiff) * dProportion;

  m_CurrentPosition += dMove;

  // Check for overshoot
  if(  (dMove > 0 && m_CurrentPosition >= m_IntendedPosition) ||
       (dMove < 0 && m_CurrentPosition <= m_IntendedPosition) )
  {
    m_CurrentPosition = m_IntendedPosition;
  }

  m_servo.write(m_CurrentPosition);
  PrintServoPos(m_nPin, m_CurrentPosition);
  return true;
}

bool ServoMotor::SetPosition(int nDeg)
{
  if(nDeg < 0)
    return false;

  if(nDeg > 180)
    return false;
  
  if(m_CurrentPosition == nDeg)
    return false;

  m_CurrentPosition = nDeg;
  m_servo.write(m_CurrentPosition);

  return true;
}


void ServoMotor::PrintServoPos(int nServo, int nPos)
{
  // Serial.print("Servo: ");Serial.print(nServo);Serial.print(" moving to: ");Serial.println(nPos);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////




  