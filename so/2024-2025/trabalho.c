#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>



int main()
{
    int pid, x = 4;

    pid = fork();
    if (0 == pid) 
    { 
         fork(); 
         x=x+2; 
    }
    else 
    { 
         x--;
    }
     printf("x=%d\n", x);
}
