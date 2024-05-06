#include <stdio.h>
#include <stdbool.h>

#include "Default_CLI.h"

int Version_Shell(int argc, char **argv) 
{
    //printf("Build: [" SHELL_VERSION ":" USER_REPO_VERSION "] - [" BUILD_USER "@" BUILD_HOST "] - " __DATE__ " - " __TIME__ "\n");
    printf("Build: [0.0.0: ?] - [ GSI@Mac OS X] - 5 April 2024 - 10:46\n");
    return 0;
}

