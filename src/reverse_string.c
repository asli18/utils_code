#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 0
#define SWAP(a, b)          ((((a) ^ (b))) && (((b) ^= (a) ^= (b)), ((a) ^= (b))))

static void reverse_str(char *s)
{
    const unsigned int len = strlen(s);
    char *e = s + len - 1;

    while (e > s) {
        SWAP(*s, *e);
        s += 1;
        e -= 1;
    }
}
#else
static void swap(char *a, char *b)
{
    register char tmp = *b;

    *b = *a;
    *a = tmp;
}

static void reverse_str(char *s)
{
    const unsigned int len = strlen(s);
    char *e = s + len - 1;

    while (e > s) {
        swap(s++, e--);
    }
}
#endif

int test_3(void)
{
    char s0[] = "1";
    char s1[] = "12";
    char s2[] = "123456";
    char s3[] = "1234567";

    printf("%s %s %s %s\n", s0, s1, s2, s3);

    reverse_str(s0);
    reverse_str(s1);
    reverse_str(s2);
    reverse_str(s3);
    printf("%s %s %s %s\n", s0, s1, s2, s3);

    reverse_str(s0);
    reverse_str(s1);
    reverse_str(s2);
    reverse_str(s3);
    printf("%s %s %s %s\n", s0, s1, s2, s3);

    return 0;
}

