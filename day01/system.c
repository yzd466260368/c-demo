#include <stdio.h>

int main(int argc, char const *argv[])
{
    unsigned int x = 0x01020304;

    unsigned char *p = (unsigned char *) &x;

    if (*p == 0x04)
    {
        printf("little endian\n");
    }else if (*p == 0x01)
    {
        printf("big endian\n");
    }else{
        printf("unknown\n");
    }
    
    




    return 0;
}
