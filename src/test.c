#include <stdio.h>
#include <stdint.h>
#include <string.h>


int test_0(void) {

    {
        uint32_t num = 1;
        char *ptr = (char *)&num;

        if (*ptr == 1) {
            printf("little-endian\n");
        } else {
            printf("big-endian\n");
        }
        printf("-----------------------\n");
    }
    {   /* 64bit unsigned long */
        unsigned long foo = ~0u;
        unsigned long bar = ~0ul;

        printf("unsigned long foo = ~0u;\n");
        printf("unsigned long bar = ~0ul;\n");
        printf("\noutput:\n");
        printf("foo %016lx\n", foo);
        printf("bar %016lx\n", bar);
        printf("-----------------------\n");
    }
    {
        int a[5];                  // if address of a is 0x1000
        int *p1 = (int *)(a + 1);  // p1 should be 0x1004
        int *p2 = (int *)(&a + 1); // p2 should be 0x1014
        printf("int a[5]; // if address of a is 0x1000\n");
        printf("int *p1 = (int *)(a + 1);  // p1 should be 0x1004\n");
        printf("int *p2 = (int *)(&a + 1); // p2 should be 0x1014\n");
        printf("\noutput:\n");
        printf("sizeof(a)=%ld, sizeof(&a)=%ld\n", sizeof(a), sizeof(&a));
        printf("a  0x%016lx\n", (uint64_t)a);
        printf("p1 0x%016lx (int *)(a + 1) = a[1]\n", (uint64_t)p1);
        printf("p2 0x%016lx\n", (uint64_t)p2);
        printf("-----------------------\n");
    }
    {
        unsigned char s[3], mask = 0x3F;
        short a, b, c;

        s[0] = 0xE6;
        s[1] = 0xB1;
        s[2] = 0x89;

        a = (short)(s[2] & mask);
        b = (short)(((s[1] & mask) << 8) | (s[2] & mask));
        c = (short)((s[0] << 12) | ((s[1] & mask) << 6) | (s[2] & mask));

        printf("unsigned char s[3], mask = 0x3F;\n");
        printf("short a, b, c;\n");
        printf("s[0] = 0xE6;\n");
        printf("s[1] = 0xB1;\n");
        printf("s[2] = 0x89;\n");
        printf("a = (short)(s[2] & mask);                                        \n");
        printf("b = (short)(((s[1] & mask) << 8) | (s[2] & mask));               \n");
        printf("c = (short)((s[0] << 12) | ((s[1] & mask) << 6) | (s[2] & mask));\n");

        printf("\noutput:\n");
        printf("a 0x%04x\n", a);
        printf("b 0x%04x\n", b);
        printf("c 0x%04x\n", c);
    }

    return 0;
}

int test_1(void) {
    {
        int array[8] = { -1, 0, 1, 2, 3, 4, 5, 6};
        char string[8] = { -1, 0, 1, 2, 3, 4, 5, 6};
        short *data = (short *)string;
        unsigned short *data2 = (unsigned short *)array;

        printf("int array[8] = { -1, 0, 1, 2, 3, 4, 5, 6};\n" \
               "char string[8] = { -1, 0, 1, 2, 3, 4, 5, 6};\n" \
               "short *data = (short *)string;\n" \
               "unsigned short *data2 = (unsigned short *)array;\n\n");

        printf("\noutput:\n");
        printf("sizeof(char)        %lu\n", sizeof(char));
        printf("sizeof(short)       %lu\n", sizeof(short));
        printf("sizeof(int)         %lu\n", sizeof(int));
        printf("sizeof(long)        %lu\n", sizeof(long));
        printf("sizeof(long int)    %lu\n", sizeof(long int));
        printf("sizeof(long long)   %lu\n", sizeof(long long));
        printf("sizeof(float)       %lu\n", sizeof(float));
        printf("sizeof(double)      %lu\n", sizeof(double));
        printf("sizeof(string)      %lu\n", sizeof(string));
        printf("sizeof(array)       %lu\n", sizeof(array));
        printf("sizeof(*array)      %lu\n", sizeof(*array));
        printf("sizeof(string[12])  %lu\n", sizeof(string[12]));
        printf("sizeof(&string[0])  %lu\n", sizeof(&string[0]));
        printf("sizeof(data)        %lu\n", sizeof(data));
        printf("sizeof(*data)       %lu\n", sizeof(*data));
        printf("\n");
        printf("array  address: 0x%016lx\n", (uint64_t)&array);
        printf("string address: 0x%016lx\n", (uint64_t)&string);
        printf("data   address: 0x%016lx\n", (uint64_t)data);
        printf("data2  address: 0x%016lx\n", (uint64_t)data2);
        printf("\n");
        printf("string[0]      %6d\n", string[0]);
        printf("string[8]      %6d (out of range)\n", string[8]);
        printf("array[7]       %6d\n", array[7]);
        printf("data[0]        %6d 0x%04x\n", data[0], data[0]);
        printf("data[7]        %6d 0x%04x (out of range)\n", data[7], data[7]);
        printf("data2[0]       %6d 0x%04x\n", data2[0], data2[0]);
        printf("data2[3]       %6d 0x%04x\n", data2[3], data2[3]);
        printf("data2[4]       %6d 0x%04x\n", data2[4], data2[4]);
        printf("data2[5]       %6d 0x%04x\n", data2[5], data2[5]);
        printf("*(data2 + 9)   %6d 0x%04x\n", *(data2 + 9), *(data2 + 9));
        printf("*(data + 1)    %6d 0x%04x\n", *(data + 1), *(data + 1));
        printf("\n");

        *data = 0x1234;
        printf("*data = 0x1234;\n");
        printf("string[0]      %6d 0x%02x\n", string[0], string[0]);

    }

    return 0;
}

// type promotion
int test_2(void) {
    {
        printf("unsigned char a = 6;\n" \
               "char b = -20;\n" \
               "(a + b > 6) ? printf(\"> 6\") : printf(\"<= 6\");\n");

        printf("\noutput:\n");
        unsigned char a = 6;
        char b = -20;
        (a + b > 6) ? printf("> 6") : printf("<= 6");
    }
    printf("\n-----------------------\n");
    {
        printf("unsigned int a = 6;;\n" \
               "int b = -20;\n" \
               "(a + b > 6) ? printf(\"> 6\") : printf(\"<= 6\");\n");

        printf("\noutput:\n");
        unsigned int a = 6;
        int b = -20;
        (a + b > 6) ? printf("> 6") : printf("<= 6");
    }
    printf("\n");

    return 0;
}

// 9 x 9 Multiplication table
int test_5(void) {
    uint32_t a = 9, b = 9;

    do {
        printf("%u x %u = %2u\n", a, b, (a * b));

        if (--b == 0) {
            b = 9;
            a -= 1;
            printf("\n");
        }
    } while (a);

    return 0;
}

// MSB->LSB, LSB->MSB
int test_6(void) {
    printf("Reverse bits MSB->LSB LSB->MSB (32-bit)\n");
    {
        uint32_t target = 0x815a070f;
        uint32_t output = 0;

        for (uint32_t i = 0; i < 32; ++i) {
            output |= ((target >> i) & 1) << (31 - i);
        }
        printf("ori: 0x%08x\n", target);
        printf("rev: 0x%08x\n", output);
    }

    printf("Reverse bits MSB->LSB LSB->MSB (64-bit)\n");
    {
        uint64_t target = 0xffff7070815a070f;
        uint64_t output = 0;

        for (uint32_t i = 0; i < 64; ++i) {
            output |= ((target >> i) & 1) << (63 - i);
        }
        printf("ori: 0x%016lx\n", target);
        printf("rev: 0x%016lx\n", output);
    }

    return 0;
}

/*
 * C program to print following pyramid
 *     1
 *    121
 *   12321
 *  1234321
 * 123454321
 */
int test_7(void)
{
#define LOOP_NUM        (5)

    uint32_t y, x;

    printf("Full pyramid of numbers:\n");

    /* Run parent loop */
    for (y = LOOP_NUM; y > 0; --y) {
        /* Print space */
        for(x = y - 1; x > 0; --x) {
            printf(" ");
        }

        /* Run loop to print first part of row */
        for(x = 1; x <= (LOOP_NUM - y) + 1; ++x) {
            printf("%d", x);
        }

        /* Run loop to print second part of row */
        for(x = (LOOP_NUM - y); x > 0; --x) {
            printf("%d", x);
        }

        printf("\n");
    }

    printf("\nInverted:\n");

    /* Run parent loop */
    for (y = LOOP_NUM; y > 0; --y) {
        /* Print space */
        for (x = LOOP_NUM - y; x != 0; --x) {
            printf(" ");
        }

        /* Run loop to print first part of row */
        for (x = 1; x <= y; ++x) {
            printf("%d", x);
        }

        /* Run loop to print second part of row */
        for (x = y - 1; x >= 1; --x) {
            printf("%d",x);
        }

        printf("\n");
    }

    return 0;
}

