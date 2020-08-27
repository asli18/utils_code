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

void reverse_str0(char *s)
{
    const unsigned int len = strlen(s);
    char *e = s + len - 1;

    while (e > s) {
        SWAP(*s, *e);
        s += 1;
        e -= 1;
    }
}

void reverse_str1(char *s)
{
    const unsigned int len = strlen(s);
    char *e = s + len - 1;

    while (e > s) {
        swap(s++, e--);
    }
}

int main(void)
{
    char s0[] = "1";
    char s1[] = "12";
    char s2[] = "123456";
    char s3[] = "1234567";

    printf("%s %s %s %s\n", s0, s1, s2, s3);

    reverse_str0(s0);
    reverse_str0(s1);
    reverse_str0(s2);
    reverse_str0(s3);
    printf("%s %s %s %s\n", s0, s1, s2, s3);

    reverse_str1(s0);
    reverse_str1(s1);
    reverse_str1(s2);
    reverse_str1(s3);
    printf("%s %s %s %s\n", s0, s1, s2, s3);

    return 0;
}

