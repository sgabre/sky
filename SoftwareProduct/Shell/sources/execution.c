#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "List.h"

#define MAX_NUM_PARAMS       32
#define LINE_BUFF_SIZE 32

#define E_SHELL_NOT_INITIALIZED         1
#define E_SHELL_CMD_WRONG_PARAMETERS    0xFF000001
#define E_SHELL_CMD_NOT_FOUND           0xFF000002
#define E_SHELL_OPEN_REDIR_FILE     1


typedef enum osPriority_t
{
  osPriorityNormal
}osPriority_t;

typedef struct osThreadAttr_t
{
  const char *	name;
  uint32_t	stack_size;
  osPriority_t	priority;
}osThreadAttr_t;

/// struct passed to each handler function.
typedef struct
{
  char    *pacCommand;            //< command
  char    *pacCmdLine;            //< whole command line
  char    *pacCurrentDir;         //< current directory
  FILE    *pfOut;                 //< stream to output debug output (fprintf)
  FILE    *pfIn;                  //< stream to read input from
  List_ts  *ptParams;              //< t_List with parameters passed to command
  uint8_t    ucNumParams;           //< number of parameters in t_List
} t_ShellCmdContext;

/*== Function Types ==========================================================*/
typedef uint32_t (*pfCmdHandler)(const t_ShellCmdContext *c_ptContext);

typedef struct
{
  char           acName[16];     ///< a command must not be longer than 15 chars
  pfCmdHandler   pfHandler;      ///< function pointer of handler
  const char    *c_pacSummary;   ///< help text for summary
  const char    *c_pacUsage;     ///< help text for usage (parameter description)
} t_CommandEntry;

uint32_t mShell_Exec(const char* c_pacCmdLine);


static void mShell_SubThread(void *arg)
{
  char  *pacCmdLine = (char *)arg;
  
//  printf("\r\nThread %s with osThreadId 0x%08X started.\r\n", OSEXT_ThreadGetName(OSEXT_ThreadGetId()), OSEXT_ThreadGetId());
  
  mShell_Exec((char *)pacCmdLine);
  free(pacCmdLine);
  
  //printf("\r\nThread %s with osThreadId 0x%08X terminated.\r\n", OSEXT_ThreadGetName(OSEXT_ThreadGetId()), OSEXT_ThreadGetId());
  
  //mSHELL_DisplayPrompt();
}

uint32_t mShell_Exec(const char* c_pacCmdLine)
{
  uint32_t  ulErr = 0;

  // used to split the string into a space delimited list
  const char  *c_apacSearch[] = {" ", NULL};

  // split the command line
  List_ts* ptLstParams = STR_split(c_pacCmdLine, c_apacSearch, LINE_BUFF_SIZE, MAX_NUM_PARAMS + 1);

  bool  bOwnThread = false;
  
  // Check, if the last "parameter" starts with a "&" - this would cause to start the function in an own thread.
  // Perform the check before "parameter grouping" to be able to easily escape a real & parameter by "&".
  if (List_ItemCount(ptLstParams) >= 1)
  {
    uint16_t   unLastItem = List_ItemCount(ptLstParams) - 1;
    char    *pacLastParam = (char *)List_GetItem(ptLstParams, unLastItem, NULL);
    if (pacLastParam[0] == '&')
    {
      uint32_t   ulPrio = 0;
      char    *pacCmdLine = NULL;

      sscanf(pacLastParam, "&(%d)", &ulPrio);
      if ((ulPrio < 1) || (ulPrio > 254))
        ulPrio = 20;

      // Delete &(x) from list
      List_DeleteItem(ptLstParams, unLastItem);

      pacCmdLine = List_ToString(ptLstParams, " ");

      // the space delimited list we no longer use
      List_Free(ptLstParams);

      // Create new thread
      //osThreadAttr_t ThreadAttribut = {.name ="Shell_SubThread", .stack_size=1024, .priority = osPriorityNormal};
      //osThreadNew(mShell_SubThread, pacCmdLine, &ThreadAttribut);
      
      bOwnThread = true;
    }
  }

  if (!bOwnThread)
  {
    t_ShellCmdContext   tContext;
    uint16_t              unI;
    bool                bQuoteActive = false;
    char               *pacCurParam = NULL;

    // build the context base - it will be overgiven to the called function handler
    // ptParams is a LIST, which will be filled below with the "parameter list" (parameters overgiven to the shell function)
    memset(&tContext, 0, sizeof(t_ShellCmdContext));
    tContext.pacCmdLine = (char *)c_pacCmdLine;
     tContext.pacCurrentDir = strcpy(tContext.pacCurrentDir, m_pacDir);
    tContext.ptParams = List_Create(MAX_NUM_PARAMS);
    tContext.pfIn = stdin;
    tContext.pfOut = stdout;

    // go through each element of the space delimited command line
    // and fill our parameter list -> group parameters, which are surrounded by "..."
    for (unI = 0; unI < List_ItemCount(ptLstParams); unI++)
    {
      // The first element is the command itself -> copy it as c_pacCommand
      if (unI == 0)
        tContext.pacCommand = strcpy(tContext.pacCommand, (char *)List_GetItem(ptLstParams, unI, NULL) );
      else
      {
        // now parse for open quotes (the first character of a "parameter"
        // and closing quotes (the last character of a paramter)
        // -> combine space delimited elements into one parameter, if quotes are around them
        const char * c_pacParam = (char *)List_GetItem(ptLstParams, unI, NULL);
        if (!bQuoteActive)
        {
          if (c_pacParam[0] != '\"')
            // no " active -> just take the element and at it to our parameter list
            List_AddItem(tContext.ptParams, (uint8_t *)c_pacParam, strlen(c_pacParam) + 1);
          else
            // quote active -> manage it below.
            bQuoteActive = true;
        }

        if (bQuoteActive)
        {
          // We have an opened quote.
          // just append the element to pacCurParam -> it will be added to our parameter list, as soon as the closing quote arrives
          uint16_t  unLen;
          pacCurParam = (pacCurParam == NULL) ? strcpy(pacCurParam, c_pacParam + 1) : STR_cat(pacCurParam, 2, " ", c_pacParam);
          unLen = strlen(pacCurParam);
          if (pacCurParam[unLen - 1] == '\"')
          {
            // here we have the closing quote -> add the whole construct as single parameter (without last quote character)
            pacCurParam[unLen - 1] = 0;
            List_AddItem(tContext.ptParams, (uint8_t *)pacCurParam, unLen);
            bQuoteActive = false;
            free(pacCurParam);
            pacCurParam = NULL;
          }
        }
      }
    }

    // the space delimited list we no longer use
    List_Free(ptLstParams);

    // Check, if the last but one entry is a ">" or ">>" - in this case, we have to redirtect the output to a file instead of stdout
    if (List_ItemCount(tContext.ptParams) >= 2)
    {
      bool     bOutputRedirect = false;
      uint16_t   unLastItem = List_ItemCount(tContext.ptParams) - 1;
      char    *pacRedir = (char *)List_GetItem(tContext.ptParams, unLastItem - 1, NULL);
      char    *pacFile = (char *)List_GetItem(tContext.ptParams, unLastItem, NULL);

      // Operator > will rewrite a new file.
      // Operator >> will append to an existing file; if it does not exist, create a new one.
      if (strcmp(pacRedir, ">") == 0)
      {
        tContext.pfOut = fopen(pacFile, "w");
        bOutputRedirect = true;
      }
      else if (strcmp(pacRedir, ">>") == 0)
      {
        tContext.pfOut = fopen(pacFile, "a");
        bOutputRedirect = true;
      }

      if (bOutputRedirect)
      {
        if (!tContext.pfOut)
        {
          printf("\r\nERROR: Cannot open redirection file '%s'\r\n", pacFile);
          ulErr = E_SHELL_OPEN_REDIR_FILE;
        }
        else
        {
          // remove these two entries - the command shall not get these as parameters.
          List_DeleteItem(tContext.ptParams, unLastItem);
          List_DeleteItem(tContext.ptParams, unLastItem - 1);
        }
      }
    }

    if (!ulErr)
    {
      // Pointer to Command Entry in the dynamic LIST
      t_CommandEntry  *ptEntry = NULL;
      t_CommandEntry   tEntry;

      // set ucNumParams to have faster access to the number of parameters in the handler functions
      tContext.ucNumParams = List_ItemCount(tContext.ptParams);

      // Protect the Command List (no one shall be able to add a new command and resort the list...)
      //osMutexAcquire(m_ptOsMutIdCmdList, osWaitForever);

      // Search the command in the registered command set
      ptEntry = (t_CommandEntry *)List_SearchStrItem(m_ptCmdList, tContext.pacCommand, 0, NULL);
      memcpy(&tEntry, ptEntry, sizeof(t_CommandEntry));
      //osMutexRelease(m_ptOsMutIdCmdList);

      // If Command has been found in the list...
      if (ptEntry)
      {
        uint32_t  ulErr;

        printf("\r\n");

        // Command found -> execute it
        ulErr = tEntry.pfHandler(&tContext);

        // Check return code of called function
        if (ulErr)
        {
          if (ulErr == E_SHELL_CMD_WRONG_PARAMETERS)
            fprintf(tContext.pfOut, "\r\nERROR: Wrong/invalid parameters specified! Check usage below:\r\nUsage: %s\r\n", tEntry.c_pacUsage);
          else
            fprintf (tContext.pfOut, "\r\nERROR: '%s' has been terminated with return value %d (0x%X)\r\n", tEntry.acName, ulErr, ulErr);
        }
      }
      else
      {
        ulErr = E_SHELL_CMD_NOT_FOUND;

        // if just enter has been pressed -> do not display "command not found" error.
        if (strlen(c_pacCmdLine) > 0)
          printf("\r\nERROR: Command not found!");
      }
    }

    // if output was redirected to a file, close it!
    if (tContext.pfOut && tContext.pfOut != stdout)
      fclose(tContext.pfOut);    
    else
      clearerr(stdout);

    // Free all allocations used in handler context
    if (tContext.ptParams)
      List_Free(tContext.ptParams);
    if (tContext.pacCurrentDir)
      free(tContext.pacCurrentDir);
    if (tContext.pacCommand)
      free(tContext.pacCommand);
  }

  return ulErr;
}



