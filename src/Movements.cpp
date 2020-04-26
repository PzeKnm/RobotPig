


#include "Movements.h"
#include "Arduino.h"



CMovements::CMovements()
{  
 m_bBusy = false;
} 


CMovements::~CMovements()
{

} 

  
void CMovements::Init()
{
  m_set.Init();

  for(int n = 0; n < NUMBER_OF_MOVEMENTS; n++)
  {
    //// Serial.print("Adding set: ");Serial.println(n);
    uint16_t addr = (uint16_t)&movements[n];
    SetCommandSet(n, addr);
/*
    int nLineCount = GetCommandLineTotalCount(n);
    Serial.print("nLineCount: ");Serial.println(nLineCount);
    for(int nLine = 0; nLine < nLineCount; nLine++)
    {
      const char* pLine = GetCommandLine(n, nLine);
      Serial.print("pLine: ");Serial.println(pLine);
    }*/
  }


}


void CMovements::SetCommandSet(uint8_t ix, uint16_t addrCmd)
{
   m_CommandSets[ix] = addrCmd;  

/*Serial.println("CMovements::SetCommandSet");
  int nLineCount = GetCommandLineTotalCount(ix);
  for(int nLine = 0; nLine < nLineCount; nLine++)
  {
    const char* pLine = GetCommandLine(ix, nLine);
    Serial.print("pLine: ");Serial.println(pLine);
  }
*/

  /*
  Serial.println("CMovements::SetCommandSet");
  char * ptr = (char *) pgm_read_word (addrCmd);

  int nLen = strlen_P(ptr);
  Serial.print("length of buffer: ");Serial.println(nLen);

  int nLineCount = nLen / COMMAND_LINE_LENGTH;
  Serial.print("nLineCount: ");Serial.println(nLineCount);
  for(int nLine = 0; nLine < nLineCount; nLine++)
  {
    char buffer [COMMAND_LINE_LENGTH + 1];
    strncpy_P(buffer, ptr + (nLine * COMMAND_LINE_LENGTH), COMMAND_LINE_LENGTH);
    buffer[COMMAND_LINE_LENGTH] = '\0';
    Serial.println (buffer);
  }*/  
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
  if(nCmd >= NUMBER_OF_MOVEMENTS)
  {
    Serial.print("Invalid command: "); Serial.println(nCmd);
    return;
  }

  m_nCurrentCmd = nCmd;
  m_ixCurrentLine = 0;

  const char* pszCmd = GetCommandLine(m_nCurrentCmd, m_ixCurrentLine);
  //// Serial.print("Command: "); Serial.println(pszCmd);

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
  char * ptr = (char *) pgm_read_word (m_CommandSets[nCmd]);
  char buffer [COMMAND_LINE_LENGTH + 1];
  strncpy_P(buffer, ptr + (ixLine * COMMAND_LINE_LENGTH), COMMAND_LINE_LENGTH);
  buffer[COMMAND_LINE_LENGTH] = '\0';
  strcpy(m_bufferCurrentLine, buffer);
  // Serial.print("CMovements::GetCommandLine: ");Serial.println(m_bufferCurrentLine);
  return m_bufferCurrentLine;
}

int CMovements::GetCommandLineTotalCount(int nCmd)
{
  char * ptr = (char *) pgm_read_word (m_CommandSets[nCmd]);
  int nLen = strlen_P(ptr);
  int nLineCount = nLen / COMMAND_LINE_LENGTH;
  //// Serial.print("nLineCount: ");Serial.println(nLineCount);
  return nLineCount;
}


 