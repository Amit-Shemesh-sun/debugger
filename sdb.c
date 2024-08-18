#include <sys/ptrace.h>
#include <stdio.h>
#include <unistd.h>
#include "sdb.h"
#include <sys/wait.h>
#include <string.h>


#define CONTINUE ("continue")
#define QUIT ("q")
#define BREAK ("break")

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
        printf("sdb>");
        fgets(command, sizeof(command), stdin);
        if (!strncmp(CONTINUE, command, sizeof(CONTINUE) - 1)) {
            continue_program(pid);
        }
        else if (!strncmp(BREAK, command, sizeof(QUIT) - 1)) {
            set_break(pid, command);
        }
        else if (!strncmp(QUIT, command, sizeof(QUIT) - 1)) {
            return;
        }
    }
}


void continue_program(int pid) {
    int wait_status;
    int options = 0;

    ptrace(PTRACE_CONT, pid, NULL, NULL);
    waitpid(pid, &wait_status, options);
}

void set_break(int pid, char command[]) {
    char token[]  = strtok(strtok(command, ' '), ' ');
    
}