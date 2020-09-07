#include <stdio.h>


int main(void)
{
    int y = 20, x = 35;

    x = y++ + x++;
    printf("%d%d\n", x, y);

    y = ++y + ++x;
    printf("%d%d\n", x, y);

#if 0 /* Output */
    5521
    5678
#endif

    return 0;
}

