#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "dshlib.h"

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    // Initialize the command list
    clist->num = 0;

    // Split the command line by pipes
    char *token = strtok(cmd_line, PIPE_STRING);
    while (token != NULL && clist->num < CMD_MAX) {
        // Remove leading and trailing spaces
        while (isspace(*token)) token++;
        char *end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) end--;
        *(end + 1) = '\0';

        // Split into executable and arguments
        char *exe = token;
        char *args = strchr(token, SPACE_CHAR);
        if (args != NULL) {
            *args = '\0'; // Separate executable from arguments
            args++;
            while (isspace(*args)) args++; // Remove leading spaces from args
        }

        // Check for oversized executable or arguments
        if (strlen(exe) >= EXE_MAX || (args != NULL && strlen(args) >= ARG_MAX)) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        // Populate the command structure
        strcpy(clist->commands[clist->num].exe, exe);
        if (args != NULL) {
            strcpy(clist->commands[clist->num].args, args);
        } else {
            clist->commands[clist->num].args[0] = '\0';
        }

        clist->num++;
        token = strtok(NULL, PIPE_STRING);
    }

    // Check for too many commands
    if (token != NULL) {
        return ERR_TOO_MANY_COMMANDS;
    }

    // Check for empty command line
    if (clist->num == 0) {
        return WARN_NO_CMDS;
    }

    return OK;
}