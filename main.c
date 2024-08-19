#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include "sdb.h"


int main(int argc, char* argv[]) {
    char* program_name = argv[1];
    int pid = 0;

    if (argc < 2) {
        fputs("Please specify program", stderr);
        return -1;
    }

    pid = fork();
    if (0 == pid) {
        run_debugee(program_name);
    }
    else if (pid >= 1) {
        run_debuger(pid, program_name);
    }
}
