#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static unsigned int count_bit(unsigned int data)
{
    unsigned int c = 0;

    while (data) {
        data &= (data - 1);
        c += 1;
    }
    return c;
}


int main(void)
{
    unsigned int data = ~0;

    do {
        printf("data 0x%08x %u\n", data, count_bit(data));
        data >>= 1;
    } while (data);

    return 0;
}

