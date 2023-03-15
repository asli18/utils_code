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

// reverse string
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


// Reverse words in a string
void reverse_words(char *str) {
    int len = strlen(str);
    char *start = str;

    for (int i = 0; i < len; i++) {
        if (str[i] == ' ') {
            // Reverse the word between start and i-1
            char *end = str + i - 1;
            while (start < end) {
                swap(start++, end--);
            }
            start = str + i + 1;
        }
    }

    // Reverse the last word
    char *end = str + len - 1;
    while (start < end) {
        swap(start++, end--);
    }
}

int test_4(void) {
    char s0[] = "hello world, how are you?";
    char s1[] = "1234 abc, xyz";

    printf("ori: %s\n", s0);
    reverse_words(s0);
    printf("rev: %s\n", s0);

    printf("ori: %s\n", s1);
    reverse_words(s1);
    printf("rev: %s\n", s1);

    return 0;
}

