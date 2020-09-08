#include <stdio.h>

/*
 *C program to print following pyramid
 *    1
 *   121
 *  12321
 * 1234321
 *123454321
 */

#define LOOP_NUM        (5)

int main(void)
{
    int i, j;

    printf("Print pyramid:\n");

    /* Run parent loop */
    for (i = LOOP_NUM; i != 0; --i) {
        /* Print space */
        for(j = i - 1; j != 0; --j)
            printf(" ");

        /* Run loop to print first part of row */
        for(j = 1; j <= (LOOP_NUM - i) + 1; ++j)
            printf("%d", j);

        /* Run loop to print second part of row */
        for(j = (LOOP_NUM - i); j != 0; --j)
            printf("%d", j);

        printf("\n");
    }

    printf("\nReverse:\n");

    /* Run parent loop */
    for (i = LOOP_NUM; i != 0; --i) {
        /* Print space */
        for (j = LOOP_NUM - i; j != 0; --j)
            printf(" ");

        /* Run loop to print first part of row */
        for (j = 1; j <= i; ++j)
            printf("%d", j);

        /* Run loop to print second part of row */
        for (j = i - 1; j >= 1; --j)
            printf("%d",j);

        printf("\n");
    }

    return 0;
}

