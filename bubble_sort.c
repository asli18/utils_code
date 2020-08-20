#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SWAP(a, b)          ((((a) ^ (b))) && (((b) ^= (a) ^= (b)), ((a) ^= (b))))


static void swap(char *a, char *b)
{
    register int tmp = *b;

    *b = *a;
    *a = tmp;
}

void bubble_sort(char *a, int len)
{
    for (; len != 0; len -= 1) {
        for (int i = 0; i < (len - 1); i += 1) {
            if (a[i] > a[i + 1])
                swap(&a[i], &a[i + 1]);
        }
    }
}

int main(void)
{
    char arr[10] = {71,66,19,20,8,43,54,3,2,10};

    for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); i += 1)
        printf("%d ", arr[i]);
    printf("\n");

    bubble_sort(arr, sizeof(arr)/sizeof(arr[0]));

    for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); i += 1)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}

