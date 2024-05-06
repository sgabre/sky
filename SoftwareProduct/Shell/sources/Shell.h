
#ifndef __SHELL_H__
#define __SHELL_H__

/** @defgroup   SHELL
  * @{
  * @name       SHELL
  * @file       Shell.h
  * @brief      Configuration Session
  */
	
//@{

/*== Includes ===============================================================*/
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "List.h"
#include "../configuration/Shell_Config.h"



/*== Defines ================================================================*/


#define E_SHELL_NOT_INITIALIZED         1   //< Returned by SHELL_AddCommand, if SHELL has not been initialized yet.
#define E_SHELL_CMD_WRONG_PARAMETERS    0xFF000001
#define E_SHELL_CMD_NOT_FOUND           0xFF000002

/*== Typedefs ===============================================================*/
typedef struct ShellCommandContext_ts
{
  char*   Command;            //< command
  char*   CommandLine;            //< whole command line
  char*   Directory;         //< current directory
  FILE*   Out;                 //< stream to output debug output (fprintf)
  FILE*   In;                  //< stream to read input from
  List_ts*  Parameters;              //< t_List with parameters passed to command
  uint8_t    NumParams;           //< number of parameters in t_List
} ShellCommandContext_ts;

/*== Function Types ==========================================================*/
typedef int (*CommandCallBack_t)(int argc, char **argv);

/*== Module Interfaces ======================================================*/

//uint32_t SHELL_ADD_CMD(const char* CommandName, CommandHandlerCallBack_t aHandler, const char* aSummary, const char* acUsage);
void SHELL_ADD_CMD(const char* name, const char* help_string, CommandCallBack_t aCommand);

void Shell_Thread();


//@}

#endif  // __SHELL_H__