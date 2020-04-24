


#include "Movements.h"
#include "Arduino.h"
#include <avr/pgmspace.h>


CMovements::CMovements()
{  
 // m_pServoSet = NULL;
 m_bBusy = false;
} 


CMovements::~CMovements()
{

} 

  
void CMovements::Init()
{
  m_set.Init();
}


void CMovements::SetCommandSet(uint8_t ix, const char* pCmd)
{
  m_CommandSets[ix] = pCmd;

//  Serial.println(m_CommandSets[ix]);
//  Serial.println(strlen(m_CommandSets[ix]));
}




bool CMovements::ProcessTime(int ms)
{
  
  bool bServosBusy =  m_set.ProcessTime(ms);

  if(bServosBusy)
    return true;

  if(m_bBusy)
  {
    m_ixCurrentLine++;
    
    if(m_ixCurrentLine > GetCommandLineTotalCount(m_nCurrentCmd) - 1)
    {
      m_nCurrentCmd = 0;
      m_ixCurrentLine = 0;
      m_bBusy = false;
      return false;
    }

    const char* pszCmd = GetCommandLine(m_nCurrentCmd, m_ixCurrentLine);
    m_set.ApplyCommands(pszCmd);
    return true; 
  }

  return false;
}


bool CMovements::IsBusy()
{
  return m_bBusy;
}

  
void CMovements::DoCommand(int nCmd)
{
  m_nCurrentCmd = nCmd;
  m_ixCurrentLine = 0;

  const char* pszCmd = GetCommandLine(m_nCurrentCmd, m_ixCurrentLine);

  if(pszCmd == m_chNullCommand)
  {
    Serial.print("Command not found: "); Serial.println(nCmd);
    return;
  }
  
  m_set.ApplyCommands(pszCmd);
  m_bBusy = true; 
}

  
  
void CMovements::CueUpNextCommand(int nCmd)
{

}


const char* CMovements::GetCommandLine(int nCmd, int ixLine)
{
  /*
  Serial.println("CMovements::GetCommandLine");

  Serial.println("just referencing array...");
  Serial.println(m_CommandSets[m_nCurrentCmd]);
  Serial.println("and a line...");
  Serial.println(m_CommandSets[m_nCurrentCmd] + (ixLine * COMMAND_LINE_LENGTH));
*/
  //Serial.println(pLine);
  return m_CommandSets[m_nCurrentCmd] + (ixLine * COMMAND_LINE_LENGTH);

/*
  return (char*)pgm_read_ptr_near( m_cmd0 + nLine);

  if(nCmd == 0)
    return (char*)pgm_read_ptr_near( m_cmd0 + nLine);
  if(nCmd == 1)
    return (char*)pgm_read_ptr_near( m_cmd1 + nLine);

   if(nCmd == 0)
    return m_cmd0[nLine];
  if(nCmd == 1)
    return m_cmd1[nLine];
*/

    /*
  if(nCmd == 2)
    return m_cmd2[nLine];
  if(nCmd == 3)
    return m_cmd3[nLine];
  if(nCmd == 4)
    return m_cmd4[nLine];
  if(nCmd == 5)
    return m_cmd5[nLine];
  if(nCmd == 6)
    return m_cmd6[nLine];
  if(nCmd == 7)
    return m_cmd7[nLine];*/
  // return m_chNullCommand; 
}

int CMovements::GetCommandLineTotalCount(int nCmd)
{
  int nRet = strlen(m_CommandSets[m_nCurrentCmd]) / COMMAND_LINE_LENGTH;
  return nRet;
}


 