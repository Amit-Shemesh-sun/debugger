#include <sys/ptrace.h>
#include <stdio.h>
#include <unistd.h>
#include "sdb.h"
#include <sys/wait.h>
#include <string.h>
#include "commands.h"


#define CONTINUE ("continue")
#define QUIT ("q")
#define BREAK ("break")
#define PRINT ("print")

#define OPTION_NO_COMMAND(string, function) \
        do {\
            if (!strncmp(string , command, sizeof(string) - 1)) {\
                function(pid); } }\
        while (0)
#define OPTION_COMMAND(string, function) \
        do {\
            if (!strncmp(string , command, sizeof(string) - 1)) {\
                function(pid, command); } } \
        while (0)

void run_debugee(char* program_name) {
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
        fputs("ptrace error", stderr);
    }
    execl(program_name, program_name, NULL);
}

void run_debuger(int pid, char* program_name) {
    int wait_status;
    int options = 0;
    char command[100] = {0};

    waitpid(pid, &wait_status, options);
    while(1) {
        printf("sdb> ");
        fgets(command, sizeof(command), stdin);
        if (!strncmp(QUIT, command, sizeof(QUIT) - 1)) {
            return;
        };
        OPTION_NO_COMMAND(CONTINUE, continue_program);
        OPTION_COMMAND(BREAK, set_break);
        OPTION_COMMAND(PRINT, print);
    }
}
