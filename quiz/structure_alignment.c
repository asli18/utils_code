#include <stdio.h>


struct s1 {
    char d1;
    int arr[2];
    char d2;
};

struct s2 {
    char d1;
    char d2;
    int arr[2];
    char d3;
};

struct s3 {
    char d1;
    char d2;
    char d3;
};


int main(void)
{
    printf("%ld %ld %ld\n", sizeof(struct s1),
                            sizeof(struct s2),
                            sizeof(struct s3));
    /* Output: 16 16 3 */

    return 0;
}

