#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Executes the interactive command loop for the shell.
 */
int exec_local_cmd_loop() {
    char cmd_buff[SH_CMD_MAX];
    int rc = OK;
    cmd_buff_t cmd;

    // Initialize command buffer
    if (alloc_cmd_buff(&cmd) != OK) {
        fprintf(stderr, "Failed to allocate command buffer\n");
        return ERR_MEMORY;
    }

    while (1) {
        clear_cmd_buff(&cmd);  // Reset buffer before processing a new command

        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        // Remove the trailing newline character
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // Skip empty commands
        if (strlen(cmd_buff) == 0) {
            printf(CMD_WARN_NO_CMD);
            rc = WARN_NO_CMDS;
            continue;
        }

        // Parse the command
        if (build_cmd_buff(cmd_buff, &cmd) != OK) {
            rc = ERR_MEMORY;
            continue;
        }

        // Check for built-in commands
        Built_In_Cmds bi_cmd = match_command(cmd.argv[0]);
        if (bi_cmd != BI_NOT_BI) {
            if (bi_cmd == BI_CMD_EXIT) {
                rc = OK_EXIT;
                break;
            } else if (bi_cmd == BI_CMD_CD) {
                if (cmd.argc > 1) {
                    if (chdir(cmd.argv[1]) != 0) {
                        perror("cd");
                        rc = 1; // Set exit status for failure
                    }
                }
                continue;
            } else if (bi_cmd == BI_CMD_DRAGON) {
                print_dragon();
                continue;
            }
        }

        // Execute external command
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            rc = ERR_EXEC_CMD;
        } else if (pid == 0) {
            // Child process: Execute the command
            execvp(cmd.argv[0], cmd.argv);
            
            // If execvp fails, print error and exit with standard error code
            fprintf(stderr, "%s: command not found\n", cmd.argv[0]);
            exit(127);  // Exit code 127 is used for "command not found"
        } else {
            // Parent process: Wait for child to finish
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                rc = WEXITSTATUS(status);
                if (rc == 127) {
                    rc = ERR_EXEC_CMD;  // Standardize error reporting
                }
            }
        }

        // Explicitly return error code for invalid commands
        if (rc == ERR_EXEC_CMD) {
            return rc;
        }
    }

    // Free the command buffer
    free_cmd_buff(&cmd);

    return rc;
}

/*
 * Allocates memory for the command buffer.
 */
int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = (char *)malloc(SH_CMD_MAX);
    if (cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv)); // Initialize argument list to NULL
    cmd_buff->argc = 0;
    return OK;
}

/*
 * Frees the command buffer.
 */
int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer != NULL) {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv)); // Reset pointers
    return OK;
}

/*
 * Clears the command buffer.
 */
int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer) {
        memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    }
    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv)); // Ensure argv is null-terminated
    return OK;
}

/*
 * Parses and builds the command buffer from the input string.
 */
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    clear_cmd_buff(cmd_buff);  // Reset buffer before parsing

    strncpy(cmd_buff->_cmd_buffer, cmd_line, SH_CMD_MAX - 1);
    cmd_buff->_cmd_buffer[SH_CMD_MAX - 1] = '\0'; // Ensure null termination

    char *ptr = cmd_buff->_cmd_buffer;
    while (*ptr && cmd_buff->argc < CMD_ARGV_MAX - 1) {
        // Skip leading spaces
        while (*ptr == ' ') ptr++;

        if (*ptr == '\0') break; // End of input

        // Handle quoted strings
        if (*ptr == '"') {
            ptr++; // Skip the opening quote
            cmd_buff->argv[cmd_buff->argc++] = ptr; // Start of the quoted string

            // Find the closing quote
            while (*ptr && *ptr != '"') ptr++;

            if (*ptr == '"') {
                *ptr = '\0'; // Terminate the quoted string
                ptr++; // Move past the closing quote
            }
        } else {
            // Handle unquoted arguments
            cmd_buff->argv[cmd_buff->argc++] = ptr;

            // Find the end of the argument
            while (*ptr && *ptr != ' ') ptr++;

            if (*ptr == ' ') {
                *ptr = '\0'; // Terminate the argument
                ptr++; // Move past the space
            }
        }
    }
    cmd_buff->argv[cmd_buff->argc] = NULL; // Ensure last element is NULL

    if (cmd_buff->argc >= CMD_ARGV_MAX - 1) {
        return ERR_TOO_MANY_COMMANDS;
    }

    return OK;
}
/*
 * Matches built-in commands.
 */
Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    } else if (strcmp(input, "dragon") == 0) {
        return BI_CMD_DRAGON;
    }
    return BI_NOT_BI;
}

/*
 * Executes built-in commands.
 */
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    Built_In_Cmds bi_cmd = match_command(cmd->argv[0]);
    if (bi_cmd == BI_CMD_EXIT) {
        return BI_CMD_EXIT;
    } else if (bi_cmd == BI_CMD_CD) {
        if (cmd->argc > 1) {
            if (chdir(cmd->argv[1]) != 0) {
                perror("cd");
            }
        }
        return BI_CMD_CD;
    } else if (bi_cmd == BI_CMD_DRAGON) {
        print_dragon();
        return BI_CMD_DRAGON;
    }
    return BI_NOT_BI;
}
