/*== Includes ================================================================*/
#include <stdint.h>
#include "../configuration/Shell_Config.h"
/*== Defines =================================================================*/

/*== Enumerations ============================================================*/

typedef enum
{
	SHELL_OK_ERROR,       ///< No error occurs
	SHELL_NO_INIT_ERROR,  ///< not initialized
	SHELL_INVALID_ERROR,  ///< invalid parameter
} eShellError;


/*== Structures ==============================================================*/

/// Shell Object: Declaration of functionality
typedef struct iShell_ts
{
	/** @brief      set value of Shell
	  *
	  * @param[in]  aValue   frequency in Hz (300Hz - 3'000Hz)
	  * @return     eErr     SHELL_OK_ERROR if no error occurs                 
	  */
	eShellError (*Set) (uint16_t aValue);
	
	/** @brief      stop pwm signal, which generate sound
	  *
	  * @param      None
	  * @return     eErr     SHELL_OK_ERROR if no error occurs                 
	  */
	eShellError (*Enable) (void);
	

	/** @brief      start pwm signal, which generate sound
	  *
	  * @param      None
	  * @return     eErr     SHELL_OK_ERROR if no error occurs                 
	  */
	eShellError (*Disable) (void);
	
} iShell_ts;

/*== Internal Functions ======================================================*/


/*== Module Interfaces =======================================================*/

void iShell_Setup(void);

/** @brief      Gets the internal Shellv structure.
  * If the functions aren't initialize by Shell Module an ASSERT will occur
  *
  * Usage from Driver: iShell()->Enable = mShell_Enable;
  * Usage from Application: iShell()->Enable(...)
  *
  * @param      None
  * @return     pointer to internal Shell structure
  */
iShell_ts* iShell(void);