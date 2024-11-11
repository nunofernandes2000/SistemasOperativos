#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    printf("Good morning ");
    if (0 == fork())
        printf(" Child\n");
    else {
        printf(" Parent\n");
    }
    return 0;
}
