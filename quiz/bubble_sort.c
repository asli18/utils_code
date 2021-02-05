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
    if (len) {
        for (len -= 1; len != 0; len -= 1) {
            for (int i = 0; i < len; i += 1) {
                if (a[i] > a[i + 1])
                    swap(&a[i], &a[i + 1]);
            }
        }
    }
}

int main(void)
{
    char arr[10] = {71, 66, 19, 20, 8, 43, 54, 3, 2, 10};
    const int len = sizeof(arr) / sizeof(arr[0]);
    int i;

    for (i = 0; i < len; i += 1)
        printf("%d ", arr[i]);
    printf("\n");

    bubble_sort(arr, len);

    for (i = 0; i < len; i += 1)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}

