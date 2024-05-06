/*== Includes ================================================================*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "Shell.h"
#include "Shell_IO.h"
#include "Default_CLI.h"
#include "Archive_CLI.h"

/*== Defines =================================================================*/

//COnfiguration
//#define SHELL_TAB_COMPLETE 	0
//#define SHELL_HISTORY		0

#define MAX_NUM_PARAMS       32
#define NUMBER_OF_CMD 10
#define MAX_FILENAME_LEN    256
#define LINE_BUFF_SIZE 32
#define MAX_ARG_COUNT (LINE_BUFF_SIZE / 2)

#define  DEFAULT_DISK  "M:"


// Key codes
#define VKEY_EOL_CODE           '\0'	//END_OF_LINE
#define VKEY_SP_CODE            ' '		//SPACE
#define VKEY_TAB_CODE           '\t'	//TAB
#define VKEY_LF_CODE            '\n'	//NEW_LINE
#define VKEY_CR_CODE            '\r'	//CARRIAGE_RETURN
#define VKEY_BS_CODE            '\b'	//BACK_SPACE
#define VKEY_DEL_CODE           '\177'	//DELETE
#define VKEY_ESC_CODE           '\33'	//ESCAPE




#define VKEY_OSB_CODE '\133'		//SQUARE_BRACKET_OPEN
#define VKEY_UP_ARROW_CODE '\101'







/*== Enumerations ============================================================*/

/*== Structures ==============================================================*/


typedef struct 
{
	const char*			name;
	const char*			help;
	CommandCallBack_t 	callback;
} command_ts;

typedef struct ShellData_ts
{
	bool IsEnable;
	bool IsCommandRunning;
	void* MutexID;
	//command_ts* Table;// = CommandList
	bool HasEcho;
#ifdef SHELL_DYNAMIC_MEMORY
	char* Directory;
#else
	char Directory[MAX_FILENAME_LEN];
#endif
}ShellData_ts;

/*== Global Variables ========================================================*/
static int mTableIndex = 0;

static  command_ts __CMD_TABLE_START__[NUMBER_OF_CMD];

static ShellData_ts mShellData;


command_ts *volatile mTable = (command_ts*)&__CMD_TABLE_START__;


int Help_Shell(int argc, char **argv);
static int Quit_Shell(int argc, char **argv);

void SHELL_ADD_CMD(const char* name, const char* help_string, CommandCallBack_t aCommand) 
{
	if(mTableIndex < NUMBER_OF_CMD)
	{
		mTable[mTableIndex].name = name;
		mTable[mTableIndex].help = help_string;
		mTable[mTableIndex].callback = aCommand;
		mTableIndex++;
	}
}

static void mShell_SetDefaultDrive(void)
{
#ifdef SHELL_DYNAMIC_MEMORY
	if(mShellData.Directory == NULL)
	{
		mShellData.Directory = malloc(MAX_FILENAME_LEN);
	}
#endif
	
	strcpy(mShellData.Directory, DEFAULT_DISK);
}

static void mShell_DisplayPrompt(void)
{
	printf("\r\n\r\n%s>>", mShellData.Directory);
}

static void mShell_Delete(void) 
{
	__write_char__(VKEY_BS_CODE);
	__write_char__(VKEY_SP_CODE);
	__write_char__(VKEY_BS_CODE);
}

static void mShell_Clear(int char_count) 
{
	while (char_count) 
	{
		mShell_Delete();
		char_count--;
	}
}

static int mShell_ParseLine(char **argv, char *line_buff, int argument_size) 
{
	int argc = 0;
	int pos = 0;
	int length = strlen(line_buff);
	
	while (pos <= length) 
	{
		if (line_buff[pos] != '\t' && line_buff[pos] != VKEY_SP_CODE && line_buff[pos] != VKEY_EOL_CODE)
		{
			argv[argc++] = &line_buff[pos];
		}
		
		for (; line_buff[pos] != '\t' && line_buff[pos] != VKEY_SP_CODE && line_buff[pos] != VKEY_EOL_CODE; pos++);
		
		if (line_buff[pos] == '\t' || line_buff[pos] == VKEY_SP_CODE)
		{
			line_buff[pos] = VKEY_EOL_CODE;
		}
		
		pos++;
	}
	
	return argc;
}


bool Shell_CheckCmdParamEx(const t_ShellCmdContext* c_ptContext, const char *c_pacParamStr, int lNumArgs, ...)
{
	int      nI;
	va_list  vl;
	int      lParam;
	bool     bMatch = false;
	
	if ( c_ptContext->ucNumParams && (strcmp((char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL), c_pacParamStr) == 0) )
		{
			va_start(vl, lNumArgs);
			
			for (nI = 0; nI < lNumArgs; nI++)
				{
					lParam = va_arg(vl, int);
					if (c_ptContext->ucNumParams == lParam)
						bMatch = true;
				}
			va_end(vl);
		}
	
	return bMatch;
}

bool Shell_CheckCmdParam(const t_ShellCmdContext* c_ptContext, const char* c_pacParamStr, uint8_t ucNumParams)
{
	return Shell_CheckCmdParamEx(c_ptContext, c_pacParamStr, 1, ucNumParams);
}

#ifdef RTE_CMSIS_RTOS2
//static void mShell_Execute(int argc, char **argv) 
static void  mShell_Execute(const char *c_pacCmdLine)
{
	uint32  ulErr = 0;

	
	bool  bOwnThread = false;
	
		
	if (!bOwnThread)
		{
			t_ShellCmdContext   tContext;
			uint16              unI;
			bool                bQuoteActive = false;
			char               *pacCurParam = NULL;
			
			// build the context base - it will be overgiven to the called function handler
			// ptParams is a LIST, which will be filled below with the "parameter list" (parameters overgiven to the shell function)
			memset(&tContext, 0, sizeof(t_ShellCmdContext));
			tContext.pacCmdLine = (char *)c_pacCmdLine;
			tContext.pacCurrentDir = STR_cpy(m_pacDir);
			tContext.ptParams = LIST_Create(MAX_NUM_PARAMS);
			tContext.pfIn = stdin;
			tContext.pfOut = stdout;
			
			// go through each element of the space delimited command line
			// and fill our parameter list -> group parameters, which are surrounded by "..."
			for (unI = 0; unI < LIST_ItemCount(ptLstParams); unI++)
				{
					// The first element is the command itself -> copy it as c_pacCommand
					if (unI == 0)
						tContext.pacCommand = STR_cpy((char *)LIST_GetItem(ptLstParams, unI, NULL));
					else
						{
							// now parse for open quotes (the first character of a "parameter"
							// and closing quotes (the last character of a paramter)
							// -> combine space delimited elements into one parameter, if quotes are around them
							const char  *c_pacParam = (char *)LIST_GetItem(ptLstParams, unI, NULL);
							if (!bQuoteActive)
								{
									if (c_pacParam[0] != '\"')
										// no " active -> just take the element and at it to our parameter list
									LIST_AddItem(tContext.ptParams, (uint8 *)c_pacParam, strlen(c_pacParam) + 1);
									else
										// quote active -> manage it below.
									bQuoteActive = true;
								}
							
							if (bQuoteActive)
								{
									// We have an opened quote.
									// just append the element to pacCurParam -> it will be added to our parameter list, as soon as the closing quote arrives
									uint16  unLen;
									pacCurParam = (pacCurParam == NULL) ? STR_cpy(c_pacParam + 1) : STR_cat(pacCurParam, 2, " ", c_pacParam);
									unLen = strlen(pacCurParam);
									if (pacCurParam[unLen - 1] == '\"')
										{
											// here we have the closing quote -> add the whole construct as single parameter (without last quote character)
											pacCurParam[unLen - 1] = 0;
											LIST_AddItem(tContext.ptParams, (uint8 *)pacCurParam, unLen);
											bQuoteActive = false;
											HEAP_FREE(pacCurParam);
											pacCurParam = NULL;
										}
								}
						}
				}
			
			// the space delimited list we no longer use
			LIST_Free(ptLstParams);
			
			// Check, if the last but one entry is a ">" or ">>" - in this case, we have to redirtect the output to a file instead of stdout
			if (LIST_ItemCount(tContext.ptParams) >= 2)
				{
					bool     bOutputRedirect = false;
					uint16   unLastItem = LIST_ItemCount(tContext.ptParams) - 1;
					char    *pacRedir = (char *)LIST_GetItem(tContext.ptParams, unLastItem - 1, NULL);
					char    *pacFile = (char *)LIST_GetItem(tContext.ptParams, unLastItem, NULL);
					
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
									LIST_DeleteItem(tContext.ptParams, unLastItem);
									LIST_DeleteItem(tContext.ptParams, unLastItem - 1);
								}
						}
				}
			
			if (!ulErr)
				{
					// Pointer to Command Entry in the dynamic LIST
					t_CommandEntry  *ptEntry = NULL;
					t_CommandEntry   tEntry;
					
					// set ucNumParams to have faster access to the number of parameters in the handler functions
					tContext.ucNumParams = LIST_ItemCount(tContext.ptParams);
					
					// Protect the Command List (no one shall be able to add a new command and resort the list...)
					OSEXT_MutexAcquire(m_ptOsMutIdCmdList, osWaitForever);
					
					// Search the command in the registered command set
					ptEntry = (t_CommandEntry *)LIST_SearchStrItem(m_ptCmdList, tContext.pacCommand, 0, NULL);
					memcpy(&tEntry, ptEntry, sizeof(t_CommandEntry));
					OSEXT_MutexRelease(m_ptOsMutIdCmdList);
					
					// If Command has been found in the list...
					if (ptEntry)
						{
							uint32  ulErr;
							
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
				LIST_Free(tContext.ptParams);
			if (tContext.pacCurrentDir)
				HEAP_FREE(tContext.pacCurrentDir);
			if (tContext.pacCommand)
				HEAP_FREE(tContext.pacCommand);
		}
	
	return ulErr;
}

static void mShell_SubThread(void *arg)
{
	char  *pacCmdLine = (char *)arg;
	printf("\r\nThread %s with osThreadId 0x%08X started.\r\n", osThreadGetName(osThreadGetId()), osThreadGetId());
	//mShell_Exec((char *)pacCmdLine);
	printf("\r\nThread %s with osThreadId 0x%08X terminated.\r\n", OSEXT_ThreadGetName(osThreadGetId()), osThreadGetId());
	mShell_DisplayPrompt();
}

#else
static void mShell_Execute(int argc, char **argv) 
{
	int match_found = false;
	
	for (int i = 0; mTable[i].name != NULL; i++) 
	{
		//printf("Execute(Is I:%s ==Table[]:%s)\n", argv[0], table[i].command_name);
		if (strcmp(argv[0], mTable[i].name) == 0) 
		{
			//printf("Fount it\n");
			mShellData.IsCommandRunning = mTable[i].callback(argc, &argv[0]);
			match_found = true;
			break;
		}
	}
	
	if (match_found == false) 
	{
		//printf("\"%s\": command not found. Use \"help\" to list all command.\n", argv[0]);
		mShellData.IsCommandRunning  = -1;
	}
}
#endif





#ifdef SHELL_HISTORY
static void mShell_UpArrowHandler(char *cmd_buff, int *char_count) 
{
	if (curr_command_ptr < total_num_commands - NUM_HISTORY_ENTRIES ||curr_command_ptr == 0) 
	{
		printf("%s", cmd_buff);
		return;
	}
	
	memset(cmd_buff, 0, LINE_BUFF_SIZE);
	
	curr_command_ptr--;
	int index = (curr_command_ptr % NUM_HISTORY_ENTRIES);
	memcpy(cmd_buff, &cmd_history[index], LINE_BUFF_SIZE);
	*char_count = strlen(cmd_buff);
	
	printf("%s", cmd_buff);
}

static void mShell_DownArrowHandler(char *cmd_buff, int *char_count) 
{
	memset(cmd_buff, 0, LINE_BUFF_SIZE);
	*char_count = 0;
	if (curr_command_ptr == total_num_commands) return;
	
	curr_command_ptr++;
	int index = (curr_command_ptr % NUM_HISTORY_ENTRIES);
	memcpy(cmd_buff, &cmd_history[index], LINE_BUFF_SIZE);
	*char_count = strlen(cmd_buff);
	
	printf("%s", cmd_buff);
}

static void mShell_AddHistory(const char *cmd_str) 
{
	
	//add only if command is not empty
	if (cmd_str == NULL || strcmp(cmd_str, "") == 0) 
	{
		return;
	}
	int index = total_num_commands % NUM_HISTORY_ENTRIES;
	memcpy(&cmd_history[index], cmd_str, LINE_BUFF_SIZE);
	total_num_commands++;
	curr_command_ptr = total_num_commands;
}

static int show_history(int argc, char **argv) 
{
	uint32_t end_index = total_num_commands-1;
	uint32_t beg_index = 0;
	if (total_num_commands > NUM_HISTORY_ENTRIES) 
	{
		beg_index = total_num_commands - NUM_HISTORY_ENTRIES;
	}
	
	for (uint32_t index = beg_index; index <= end_index; ++index) 
	{
		printf("%s\n", cmd_history[index % NUM_HISTORY_ENTRIES]);
	}
	
	return 0;
}
#endif

#ifdef SHELL_TAB_COMPLETE

static int prefix_match(char *sub, int len, const char *str) 
{
	if (sub == NULL || str == NULL || len <= 0 || len > strlen(str)) 
	{
		return FALSE;
	}
	
	for (int i = 0; i<len; ++i) 
	{
		if (sub[i] != str[i]) 
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

static void mShell_TabHandler(char *cmd_buff, int *char_count) 
{
	if (cmd_buff == NULL || char_count <= 0) 
	{
		return;
	}
	
	int i = 0;
	int match_count = 0;
	int last_match = -1;
	while (table[i].command_name != NULL) 
	{ //loop over all commands
		
		//if prefix matches, print that as one of the options
		if (prefix_match(cmd_buff, *char_count, table[i].command_name)) 
		{
			match_count++;
			last_match = i;
			printf("\n%s", table[i].command_name);
		}
		
		i++;
	}
	
	// if only one match, then that's the command to be executed
	if (match_count == 1) 
	{
		memcpy(cmd_buff, table[last_match].command_name, LINE_BUFF_SIZE);
		*char_count = strlen(cmd_buff);
	}
	
	// print current line with old/updated command
	if (match_count) 
	{
		printf("\n");
		mShell_DisplayPrompt();
		printf("%s", cmd_buff);
	}
}

#endif  // SHELL_NO_TAB_COMPLETE



bool mShell_IsEchoEnable(void)
{
	return 0;
}

void mShell_Loop(void)
{
	
}

void Shell_Enable(void)
{
#ifdef RTE_CMSIS_RTOS2
	osEventFlagsSet(mShellData.IsEnable, SHELL_FLAG_ACTIVE);
#else
	mShellData.IsEnable = true;
#endif
}

void Shell_Disable(void)
{
#ifdef RTE_CMSIS_RTOS2
	osEventFlagsClear(mShellData.IsEnable, SHELL_FLAG_ACTIVE);
#else
	mShellData.IsEnable = false;
#endif
}

bool Shell_IsEnable(void)
{
#ifdef RTE_CMSIS_RTOS2
	return osEventFlagsWait(mShellData.FlagID, SHELL_FLAG_ACTIVE, osFlagsWaitAny | osFlagsNoClear, osWaitForever) == SHELL_FLAG_ACTIVE);
#else
	return mShellData.IsEnable;
#endif
}


static void mShell_Handler(void *arg)
{
	int s, argc;
	int count = 0;
	int special_key = 0;
	char c;
	
	char  LineBuffer[LINE_BUFF_SIZE];
	char* argv[MAX_ARG_COUNT];
	
	for (int i = 0; i < LINE_BUFF_SIZE; i++)
	{
		LineBuffer[i] = 0;
	}
	
	for (int i = 0; i < MAX_ARG_COUNT; i++) 
	{
		argv[i] = NULL;
	}
	
	mShell_DisplayPrompt();
	
	while (true) 
	{
		if (!Shell_IsEnable()) 
		{
			continue;
		}
			
			s = __read_char__();
			if (s != -1) 
			{
			c = (char)s;
			
			if (c == VKEY_CR_CODE || c == VKEY_LF_CODE) 
			{
				LineBuffer[count] = VKEY_EOL_CODE;
				__write_char__(VKEY_LF_CODE);
				break;
			}
			
			if (c == VKEY_DEL_CODE || c == VKEY_BS_CODE) 
			{
				if (!mShell_IsEchoEnable()) 
				{
					mShell_Delete();
					mShell_Delete();
				}
				
				// guard against the count going negative!
				if (count == 0) 
					continue;
				
				count--;
				
				LineBuffer[count] = VKEY_EOL_CODE;
				mShell_Delete ();
			} 
			else if (c == VKEY_ESC_CODE) 
			{
				special_key = 1;
				continue;
			} 
			else if (c == VKEY_OSB_CODE && special_key == 1) 
			{
				special_key = 2;
				continue;
			} 
			else if ((c == 'C' || c == 'D') && special_key != 0) 
			{
				/* Ignore left/right arrow keys */
				special_key = 0;
				continue;
			} 
			else if ((c == 'A' || c == 'B') && special_key == 2) 
			{
				if (!mShell_IsEchoEnable()) 
				{
					mShell_Clear(count + 4);
				} 
				else 
				{
					mShell_Clear(count);
				}
				/*
				* To reduce the shell size the history feature
				* is made optional. Skip history feature if
				* SHELL_NO_HISTORY is defined.
				*/
#ifdef SHELL_HISTORY
				if (c == 'A') 
				{
					mShell_UpArrowHandler(LineBuffer, &count);
				} 
				else 
				{
					mShell_DownArrowHandler(LineBuffer, &count);
				}
#endif  // SHELL_HISTORY
				special_key = 0;
				continue;
			}
#ifdef SHELL_TAB_COMPLETE
			else if (c == VKEY_TAB_CODE) 
			{
				mShell_TabHandler(LineBuffer, &count);
				continue;
			}
#endif //SHELL_TAB_COMPLETE
			else 
			{
				LineBuffer[count] = c;
				count++;
			}
				
			if (mShell_IsEchoEnable() && c != VKEY_DEL_CODE && c != VKEY_BS_CODE) 
			{
				__write_char__(c);
			}
		} 
		else 
		{
			mShell_Loop();
		}
	}
	/*
	* To reduce the shell size the history feature
	* is made optional. Skip history feature if
	* SHELL_NO_HISTORY is defined.
	*/
#ifdef SHELL_HISTORY
	mShell_AddHistory(LineBuffer);
#endif
	
	// parse the line_buff
	argc = mShell_ParseLine(argv, LineBuffer, MAX_ARG_COUNT);
	
	// execute the parsed commands
	if (argc > 0) 
	{
		mShell_Execute(argc, argv);
	}
	
}




void Shell_Setup(void) 
{    
	Shell_IO_Setup();
	
#ifdef RTE_CMSIS_RTOS2
	osEventFlagsAttr_t EventAttribut = {.name = "Shell-EventFlag"};
	mShellData.IsEnable = osEventFlagsNew(&EventAttribut);
#else
	mShellData.IsEnable = false;
#endif
	
	if (Shell_IsEnable() == false)
	{
		Shell_Enable();
	}
	
	mShell_SetDefaultDrive();
	
	//uart_init();
	//set_read_char(getchar);
	//set_write_char(putchar);
	
	// DO NOT REMOVE THESE
	SHELL_ADD_CMD("version", "Prints details of the build", Version_Shell);
	SHELL_ADD_CMD("reboot", "reboot/quit shell", Quit_Shell);
	SHELL_ADD_CMD("help", "Prints all available commands", Help_Shell);
	Archive_ShellSetup();
	
#ifdef RTE_CMSIS_RTOS2
	osThreadAttr_t ThreadAttribut = {.name = "Shell-Thread", .stack_size = 1024, priority = osPriorityNormal};
	osThreadNew(mShell_Handler, NULL, &ThreadAttribut);
#endif
}

void Shell_Thread() 
{
	Shell_Setup();
	Shell_Enable();
	
#ifndef RTE_CMSIS_RTOS2
	while (Shell_IsEnable()) 
	{
		mShell_Handler(NULL);
	}
#endif
}






int Help_Shell(int argc, char **argv) 
{
	int i = 0;
	/* Default to Verbose */
	bool verbose = true;
	
	if (argc > 1 && (strcmp(argv[1], "-l")==0)) 
	{
		verbose = false;
	} 
	else 
	{
		printf("use: help -l for list only.\n\n");
	}
	
	while (mTable[i].name != NULL) 
	{
		printf("%s", mTable[i].name);
		
		if (verbose) 
		{
			printf("\n\t");
			printf("%s", mTable[i].help);
		}
		
		printf("\n");
		i++;
	}
	
	return 0;
}

static int Quit_Shell(int argc, char **argv) 
{
	Shell_Disable();
	printf("...\n");
	return 0;
}
