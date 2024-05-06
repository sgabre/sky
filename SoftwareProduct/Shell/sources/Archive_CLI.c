#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "Shell.h"
#include "Archive_CLI.h"
#include "getopt.h"


static int mArchive_ShellHandler(int argc, char **argv);
/*--------------------------------------------------------------------------*/
/// this text will be displayed, in the shell-command overview
static const char mArchiveSummary[] = 
"The default action is to add or replace zipfile entries from list, which \r\n"
" can include the special name - to compress standard input..\r\n";

/// this text will be displayed, if this module return E_SHELL_CMD_WRONG_PARAMETERS
static const char mArchiveUsage[] =
"zip <function> \r\n"
"\r\n"
"Functions:\r\n"
"'zip create <archive_name>'     \t-- create empty file archive - don't forget to arc close after\r\n"
"'zip append <archive_name>'     \t-- open a file archive to append a file\r\n"
"'zip open <archive_name>'       \t-- open a file archive read only\r\n"
"'zip add <file_name>'           \t-- add a file to the archive\r\n"
"'zip add_comp <file_name> <comp_file_name>'      \t-- add a compressed file to the archive <comp_file_name> is deleted automatically\r\n"
"'zip close'                     \t-- close a file or archive\r\n"
"'zip extract <archive_name>'    \t-- extract all files of an archive\r\n"
"'zip list <archive_name>'       \t-- list all files of an archive\r\n"
"'zip help <file_name>'          \t-- show this help function\r\n";


void Archive_ShellSetup(void)
{
    // use module name as parameter or check with shell-cmd "help" that parameter isn't used
    //SHELL_ADD_CMD("zip", mArchive_ShellHandler, mArchiveSummary, mArchiveUsage);
    SHELL_ADD_CMD("zip", mArchiveSummary, mArchive_ShellHandler);
}

struct option ArchiveOption[] = {
    {"create", required_argument, 0, 'c'},
    {"append", required_argument, 0, 'a'},
    {"open", optional_argument, 0, 'o'},
    {"add", optional_argument, 0, 't'},
    {"close", optional_argument, 0, 'q'},
    {"extract", optional_argument, 0, 't'},
    {"list", optional_argument, 0, 'l'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
};
    
int mArchive_ShellHandler(int argc, char **argv) 
{
    int opt;
    
    clropt();
    
    //while((opt = getopt(argc, argv, ":if:lrx")) != -1)  
    while ((opt = getopt_long(argc, argv, "q:o:c:a:fs:t::h", ArchiveOption, NULL)) != -1)
    {  
        printf("option: %c\r\n",opt);
        switch(opt)  
        {  
            case 'c': 
                printf("Create Archive  %s\r\n",optarg);
                break;
            case 'a': 
                printf("append Archive  %s\r\n",optarg);
                break;
            case 'o': 
                printf("open Archive  %s\r\n",optarg);
                break;
            case 'q': 
                printf("close Archive  %s\r\n",optarg);
                break;
            case 'h': 
                printf("close Archive  %s\r\n",optarg);
                break;
            case '?':  
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  
    
    // optind is for the extra arguments 
    // which are not parsed 
    for(; optind < argc; optind++)
    {    
        printf("extra arguments: %s\n", argv[optind]); 
    }     
    
    return 0;
}


    
    return ulErr;
} 
