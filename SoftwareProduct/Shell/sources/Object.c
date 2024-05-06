
#include <stdarg.h>

#include "Shell.h"

#include "List.h"

bool Shell_CheckCmdParamEx(const ShellCommandContext_ts* aContext, const char* aParamterString, int lNumArgs, ...)
{
  int      nI;
  va_list  vl;
  int      lParam;
  bool     bMatch = false;
  
  if ( aContext->NumParams && (strcmp((char *)List_GetItem(aContext->Parameters, 0, NULL), aParamterString) == 0) )
  {
    va_start(vl, lNumArgs);
    
    for (nI = 0; nI < lNumArgs; nI++)
    {
      lParam = va_arg(vl, int);
  
      if (aContext->NumParams == lParam)
        bMatch = true;
    }
    va_end(vl);
  }
  
  return bMatch;
}

/*----------------------------------------------------------------------------*/
bool Shell_CheckCmdParam(const ShellCommandContext_ts *aContext, const char *aParamterString, uint8_t NumParams)
{
  return Shell_CheckCmdParamEx(aContext, aParamterString, 1, NumParams);
}

const char* FreeRTOS_CLIGetParameter( const char *pcCommandString, uint32_t uxWantedParameter, uint32_t *pxParameterStringLength )
{
  uint32_t uxParametersFound = 0;
  const char *pcReturn = NULL;
  
  *pxParameterStringLength = 0;
  
  while( uxParametersFound < uxWantedParameter )
    {
      /* Index the character pointer past the current word.  If this is the start
      of the command string then the first word is the command itself. */
      while( ( ( *pcCommandString ) != 0x00 ) && ( ( *pcCommandString ) != ' ' ) )
        {
          pcCommandString++;
        }
      
      /* Find the start of the next string. */
      while( ( ( *pcCommandString ) != 0x00 ) && ( ( *pcCommandString ) == ' ' ) )
        {
          pcCommandString++;
        }
      
      /* Was a string found? */
      if( *pcCommandString != 0x00 )
        {
          /* Is this the start of the required parameter? */
          uxParametersFound++;
          
          if( uxParametersFound == uxWantedParameter )
            {
              /* How long is the parameter? */
              pcReturn = pcCommandString;
              while( ( ( *pcCommandString ) != 0x00 ) && ( ( *pcCommandString ) != ' ' ) )
                {
                  ( *pxParameterStringLength )++;
                  pcCommandString++;
                }
              
              if( *pxParameterStringLength == 0 )
                {
                  pcReturn = NULL;
                }
              
              break;
            }
        }
      else
        {
          break;
        }
    }
  
  return pcReturn;
}