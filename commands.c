#include <sys/ptrace.h>
#include <stdio.h>
#include <unistd.h>
#include "sdb.h"
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <sys/user.h>


#define MAX_SYMBOL_LEN (100)
#define TEMP_MAIN_ADDRESS (0x555555555175)

#define FIND_REGISTER(register, function) \
        do {\
            if (!strncmp(string , command, sizeof(string) - 1)) {\
                function(pid); } }\
        while (0)

typedef struct breakpoint_s {
    void* address;
    char symbol[MAX_SYMBOL_LEN];
    __uint64_t original_data;
} breakpoint;

int breakpoint_amount = 0;
breakpoint* breakpoints = NULL;

void register_breakpoint(breakpoint bp);

void print_register(pid, value);

void continue_program(int pid) {
    int wait_status;
    int options = 0;

    ptrace(PTRACE_CONT, pid, NULL, NULL);
    waitpid(pid, &wait_status, options);
}

void set_break(int pid, char* command) {
    char delim = ' ';
    void* address = NULL;
    breakpoint bp;

    strtok(command, &delim);
    address = (void*)strtol(strtok(NULL, &delim), NULL, 16);

    __uint64_t data = ptrace(PTRACE_PEEKDATA, pid, address, NULL);
    __uint64_t int3 = 0xcc;
    __uint64_t data_with_int3 = (0 | int3);
    bp.address = address;
    bp.original_data = data;
    register_breakpoint(bp);

    ptrace(PTRACE_POKEDATA, pid, address, data_with_int3);
    printf("Registered breakpoint in address %p", address);
}

void register_breakpoint(breakpoint bp) {
    breakpoint_amount++;
    breakpoints = reallocarray(breakpoints, breakpoint_amount, sizeof(breakpoint));
    breakpoints[breakpoint_amount - 1] = bp;
}

void print(int pid, char command[]) {
    char delim = ' ';
    strtok(command, &delim);
    char* value;
    value = strtok(NULL, &delim);
    printf("%s", value);
    
    if ('$' == value[0]) {
        print_register(pid, value);
    }
}

void print_register(pid, value) {
    struct user_regs_struct registers;
    ptrace(PTRACE_GETREGS, pid, NULL, &registers);\

    /* TODO */
    // Add enum of registers and iterate over it 
}
