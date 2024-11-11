#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>

void main(void) {
    char * arg[3];
    arg[0] = "ls";
    arg[1] = "-la";
    arg[2] = NULL; //"~/"
    printf("Father process PID= %i before fork\n", getpid());
    if (!fork()) {
        execvp(arg[0], arg);
        printf("EXEC function failed\n");
        exit(1);
    }
    /* continuation of parent process */
    printf("Father process PID=%i\n", getpid());
    sleep(3);
    printf("Father process end PID=%i\n", getpid());
}