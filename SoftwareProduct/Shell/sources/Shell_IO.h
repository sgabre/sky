
#ifndef __SHELL_IO_H__
#define __SHELL_IO_H__

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
#include "../configuration/Shell_Config.h"


/*== Defines ================================================================*/

/*== Typedefs ===============================================================*/
/*== Function Types ==========================================================*/

/*== Module Interfaces ======================================================*/
void Shell_IO_Setup(void);
int __read_char__(void);
void __write_char__(int c);

//@}

#endif  // __SHELL_IO_H__