#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        printf("\nHello there %s!!!\n", argv[1]);
    }
    else
    {
        printf("\nHello there you unkown person you!\n");
    }
    return 0;
}