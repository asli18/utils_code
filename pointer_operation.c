#include <stdio.h>

int main(void)
{
    printf("Test 1\n");
    {
        int a[5] = {1, 2, 3, 4, 5};
        int *p = (int *)(&a + 1);


        printf("*(a + 1) 0x%x\n", *(a + 1));
        printf("*(p - 1) 0x%x\n", *(p - 1));

#if 0
        outout: https://ide.geeksforgeeks.org/LI6x2TQTnG
        *(a - 1) 0x2
        *(p - 1) 0x5
#endif
    }

    printf("\nTest 2\n");
    {
        int a[]={1,2,3,4,5,6,7,8,9,0};
        int b[2][3][4];
        int delta1, delta2;


        delta1 = (int)((char *)(a + 1) - (char *)a);
        delta2 = (int)((char *)(&a + 1) - (char *)&a);
        printf("a     = %p  , &a     = %p\n", a, &a);
        printf("a + 1 = %p %d, &a + 1 = %p %d\n\n", a + 1, delta1, &a + 1, delta2);

        delta1 = (int)((char *)(b + 1) - (char *)b);
        delta2 = (int)((char *)(&b + 1) - (char *)&b);
        printf("b     = %p   , &b     = %p\n", b, &b);
        printf("b + 1 = %p %d, &b + 1 = %p %d\n\n", b + 1, delta1, &b + 1, delta2);

        printf("*(b + 1) + 1 = %p\n", (*(b+1) + 1));
    }

    return 0;
}

