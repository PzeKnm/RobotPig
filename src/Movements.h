/* 
* CMjkBlink.h
*
* Created: 10/08/2015 17:13:58
* Author: PZEKNM
*/

#ifndef __CMovements_H__
#define __CMovements_H__

#include "ServoSet.h"
#include <avr/pgmspace.h>



const char m_cmdEyesFront [] PROGMEM =
{
  "\
S1P090,S2P090,S3P090,S4P090:1000ms"
};


//-----------------------------------------------------------------------------------
// nod of the head 
const char m_cmdNod [] PROGMEM =
{
  "\
S1P110,S2P090,S3P090,S4P090:0500ms\
S1P070,S2P090,S3P090,S4P090:0300ms\
S1P110,S2P090,S3P090,S4P090:0500ms\
S1P070,S2P090,S3P090,S4P090:0300ms\
S1P090,S2P090,S3P090,S4P090:0500ms"
};

//-----------------------------------------------------------------------------------
// shake of the head 
const char m_cmdShakeHead [] PROGMEM =
{
  "\
S1P090,S2P090,S3P090,S4P090:0500ms\
S1P090,S2P130,S3P090,S4P090:0300ms\
S1P090,S2P050,S3P090,S4P090:0500ms\
S1P090,S2P130,S3P090,S4P090:0300ms\
S1P090,S2P090,S3P090,S4P090:0500ms"
};

//-----------------------------------------------------------------------------------
// wave left arm
const char m_cmdWaveLeft [] PROGMEM =
{
  "\
S1P090,S2P090,S3P090,S4P090:0500ms\
S1P090,S2P090,S3P140,S4P090:0500ms\
S1P090,S2P090,S3P090,S4P090:0500ms"
};

//-----------------------------------------------------------------------------------
// wave right arm
const char m_cmdWaveRight [] PROGMEM =
{
  "\
S1P090,S2P090,S3P090,S4P100:0500ms\
S1P090,S2P090,S3P090,S4P050:0500ms\
S1P090,S2P090,S3P090,S4P100:0500ms"
};

//-----------------------------------------------------------------------------------
// both arms up, then down, then up again quickly
const char m_cmdHooray [] PROGMEM =
{
  "\
S1P090,S2P090,S3P090,S4P090:0500ms\
S1P090,S2P090,S3P040,S4P140:0500ms\
S1P140,S2P090,S3P140,S4P040:0300ms\
S1P140,S2P090,S3P120,S4P060:0100ms\
S1P140,S2P090,S3P140,S4P040:0100ms\
S1P140,S2P090,S3P120,S4P060:0100ms\
S1P140,S2P090,S3P140,S4P040:0100ms"
};

//-----------------------------------------------------------------------------------
// arms alternate up and down, head shake side to side
const char m_cmdPirateDance [] PROGMEM =
{
  "\
S1P090,S2P090,S3P090,S4P090:0500ms\
S1P090,S2P130,S3P150,S4P150:0500ms\
S1P090,S2P050,S3P040,S4P040:0500ms\
S1P090,S2P130,S3P150,S4P150:0500ms\
S1P090,S2P050,S3P040,S4P040:0500ms\
S1P090,S2P090,S3P090,S4P090:0500ms"
};

/*
  S1: Nod    30 - Backward,   150  - Forward
  S2: Shake  30 - Right,      150  - Left
  S3: Left   30 - Down        150  - Up
  S4: right  30 - Up          150  - Down
*/

// How long each line is
#define COMMAND_LINE_LENGTH 34
const int NUMBER_OF_MOVEMENTS = 7;

const char * const movements[NUMBER_OF_MOVEMENTS] PROGMEM = 
   { 
   m_cmdEyesFront, 
   m_cmdNod, 
   m_cmdShakeHead,
   m_cmdWaveLeft,
   m_cmdWaveRight,
   m_cmdHooray,
   m_cmdPirateDance
   };



class CMovements
{

public:

	CMovements();
	~CMovements();

    enum Movement_Type {
    EyesFront = 0,
    Nod = 1,
    ShakeHead = 2,
    WaveLeft = 3,  
    WaveRight = 4,   
    Hooray = 5,
    PirateDance = 6,
  };


  void SetCommandSet(uint8_t ix, uint16_t addrCmd);

  void Init();


  // returns true if there's still movements to be made, else false to indicate ready for next command
  bool ProcessTime(int ms);

  bool IsBusy();

  void DoCommand(int nCmd);  

  void CueUpNextCommand(int nCmd);

protected:

private:





  // Each command set has the format:
  //            1         2         3
  //   1234567890123456789012345678901234
  //  "S1P110,S2P140,S3P035,S4P100:1000ms"
  //  "S1P110,S2P140,S3P035,S4P100:1000ms"
  // with each line taking up exactly 34 chars  
  // What we're storing here are addresses in PROGMEM
  uint16_t m_CommandSets[NUMBER_OF_MOVEMENTS]; 

  char m_bufferCurrentLine [COMMAND_LINE_LENGTH + 1];

  CServoSet m_set; 

  bool m_bBusy;

  // which command is current active
  int m_nCurrentCmd;
  // which line within the command is active
  int m_ixCurrentLine;

  const char* GetCommandLine(int nCmd, int nLine);


  int GetCommandLineTotalCount(int nCmd);


  // Commands available
  const char m_chNullCommand[5] = {'N', 'o','n', 'e', '\0'};

}; 

#endif //__CMovements_H__













