#include <stdio.h>
#include <stdlib.h>

void print_fork_failed() {
    fprintf(stderr, "Fork Failed!\n");
    exit(1);
}