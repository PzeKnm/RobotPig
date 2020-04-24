/* 
* CMjkBlink.h
*
* Created: 10/08/2015 17:13:58
* Author: PZEKNM
*/

#ifndef __CMovements_H__
#define __CMovements_H__

#include "ServoSet.h"


class CMovements
{

public:

	CMovements();
	~CMovements();


  void SetCommandSet(uint8_t ix, const char* pCmd);

  void Init();


  // returns true if there's still movements to be made, else false to indicate ready for next command
  bool ProcessTime(int ms);

  bool IsBusy();

  void DoCommand(int nCmd);  

  void CueUpNextCommand(int nCmd);

protected:

private:


  // How long each line is
  #define COMMAND_LINE_LENGTH 34

  // Each command set has the format:
  //            1         2         3
  //   1234567890123456789012345678901234
  //  "S1P110,S2P140,S3P035,S4P100:1000ms"
  //  "S1P110,S2P140,S3P035,S4P100:1000ms"
  // with each line taking up exactly 34 chars  
  const char* m_CommandSets[6]; 



  CServoSet m_set; 

  bool m_bBusy;

  // which command is current active
  int m_nCurrentCmd;
  // which line within the command is active
  int m_ixCurrentLine;

  const char* GetCommandLine(int nCmd, int nLine);


  int GetCommandLineTotalCount(int nCmd);






 // const char PROGMEM m_my[5] = {'N', 'o','n', 'e', '\0'};

  // Commands available
  const char m_chNullCommand[5] = {'N', 'o','n', 'e', '\0'};




/*
  // Command 0 . Eyes front, rest position
  #define COMMAND_0_COMMAND_COUNT 1
  const char m_cmd0[COMMAND_0_COMMAND_COUNT][COMMAND_LINE_LENGTH]  =
  {
      "S01P090,S02P070,S03P090,S04P100:1000ms"
  };
*/

/*
  // Command 1 . No idea what this command does
  #define COMMAND_1_COMMAND_COUNT 10  
  const char m_cmd1[COMMAND_1_COMMAND_COUNT][COMMAND_LINE_LENGTH]  =
  {
      "S01P110,S02P140,S03P035,S04P100:1000ms",
      "S01P110,S02P140,S03P035,S04P020:0200ms",
      "S01P110,S02P140,S03P035,S04P160:0400ms",
      "S01P110,S02P140,S03P035,S04P100:0200ms",
      "S01P170,S02P140,S03P035,S04P100:0300ms",
      "S01P110,S02P140,S03P035,S04P100:0300ms",
      "S01P170,S02P140,S03P035,S04P100:0200ms",
      "S01P020,S02P140,S03P035,S04P100:0300ms",
      "S01P110,S02P140,S03P035,S04P100:0200ms",
      "S01P110,S02P140,S03P035,S04P100:2000ms",
  };
*/
/*
  // Command 2
  static const int m_cmd2Count = 10;
  char m_cmd2[m_cmd2Count][COMMAND_LINE_LENGTH]=
  {
      "S01P110,S02P140,S03P035,S04P100:1000ms",
      "S01P110,S02P140,S03P035,S04P020:0200ms",
      "S01P110,S02P140,S03P035,S04P160:0400ms",
      "S01P110,S02P140,S03P035,S04P100:0200ms",
      "S01P170,S02P140,S03P035,S04P100:0300ms",
      "S01P110,S02P140,S03P035,S04P100:0300ms",
      "S01P170,S02P140,S03P035,S04P100:0200ms",
      "S01P020,S02P140,S03P035,S04P100:0300ms",
      "S01P110,S02P140,S03P035,S04P100:0200ms",
      "S01P110,S02P140,S03P035,S04P100:2000ms",
  };

  // Command 3
  static const int m_cmd3Count = 10;
  char m_cmd3[m_cmd3Count][COMMAND_LINE_LENGTH]=
  {
      "S01P110,S02P140,S03P035,S04P100:1000ms",
      "S01P110,S02P140,S03P035,S04P020:0200ms",
      "S01P110,S02P140,S03P035,S04P160:0400ms",
      "S01P110,S02P140,S03P035,S04P100:0200ms",
      "S01P170,S02P140,S03P035,S04P100:0300ms",
      "S01P110,S02P140,S03P035,S04P100:0300ms",
      "S01P170,S02P140,S03P035,S04P100:0200ms",
      "S01P020,S02P140,S03P035,S04P100:0300ms",
      "S01P110,S02P140,S03P035,S04P100:0200ms",
      "S01P110,S02P140,S03P035,S04P100:2000ms",
  };
*/

}; 

#endif //__CMovements_H__













