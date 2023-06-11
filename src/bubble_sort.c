#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SWAP(a, b)          ((((a) ^ (b))) && (((b) ^= (a) ^= (b)), ((a) ^= (b))))


static void swap(int *a, int *b) {
    int tmp = *b;

    *b = *a;
    *a = tmp;
}

void bubble_sort(int *a, unsigned int len) {
    for (len -= 1; len > 0; --len) {
        for(unsigned int x = 0; x < len; ++x) {
            if (a[x] > a[x + 1]) {
#if 1
                swap(&a[x], &a[x + 1]);
#else
                SWAP(a[x], a[x + 1]);
#endif
            }
        }
    }
}

int test_9(void)
{
    int arr[10] = {71, 66, 19, 20, -8, 43, 54, 3, 2, -10};
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

