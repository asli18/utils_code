#include <stdio.h>


int main(void)
{
    unsigned int d0 = ~0;
    int d1 = ~0;

    printf("unsigned int data0 0x%08x >> 1 = 0x%08x\n", d0, d0 >> 1);
    printf("         int data1 0x%08x >> 1 = 0x%08x\n", d1, d1 >> 1);

#if 0 /* Output */
    unsigned int data0 0xffffffff >> 1 = 0x7fffffff
             int data1 0xffffffff >> 1 = 0xffffffff
#endif

    return 0;
}

