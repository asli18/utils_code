#include <stdio.h>
#include <stdint.h>
#include <string.h>


int test_0(void) {

    {   /* 64bit unsigned long */
        unsigned long foo = ~0u;
        unsigned long bar = ~0ul;

        printf("unsigned long foo = ~0u;\n");
        printf("unsigned long bar = ~0ul;\n");
        printf("foo %016lx\n", foo);
        printf("bar %016lx\n", bar);
        printf("-----------------------\n");
    }
    {
        int array[8] = { -1, 0, 1, 2, 3, 4, 5, 6};
        char string[8] = { -1, 0, 1, 2, 3, 4, 5, 6};
        short *data = (short *)string;
        unsigned short *data2 = (unsigned short *)array;

        printf("int array[8] = { -1, 0, 1, 2, 3, 4, 5, 6};\n" \
               "char string[8] = { -1, 0, 1, 2, 3, 4, 5, 6};\n" \
               "short *data = (short *)string;\n" \
               "unsigned short *data2 = (unsigned short *)array;\n");

        printf("-----------------------\n");
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
        printf("-----------------------\n");
        printf("string[0]           %d\n", string[0]);
        printf("string[8]           %d\n", string[8]);
        printf("array[7]            %d\n", array[7]);
        printf("data[0]             %d\n", data[0]);
        printf("data[7]             %d\n", data[7]);
        printf("data2[0]            %d\n", data2[0]);
        printf("data2[3]            %d\n", data2[3]);
        printf("data2[4]            %d\n", data2[4]);
        printf("data2[5]            %d\n", data2[5]);
        printf("*(data2 + 9)        %d\n", *(data2 + 9));
        printf("*(data + 1)         %d\n", *(data + 1));
        *data = 0x1234;
        printf("string[0]           %d\n", string[0]);

    }

    return 0;
}

int test_1(void) {
    {
        unsigned char a = 6;
        char b = -20;
        (a + b > 6) ? printf("> 6") : printf("<= 6");
    }
    printf("\n");
    {
        unsigned int a = 6;
        int b = -20;
        (a + b > 6) ? printf("> 6") : printf("<= 6");
    }
    printf("\n");

    return 0;
}

int test_2(void) {
    unsigned char s[3], mask = 0x3F;
    short a, b, c;

    s[0] = 0xE6;
    s[1] = 0xB1;
    s[2] = 0x89;

    a = (short)(s[2] & mask);
    b = (short)(((s[1] & mask) << 8) | (s[2] & mask));
    c = (short)((s[0] << 12) | ((s[1] & mask) << 6) | (s[2] & mask));

    printf("a 0x%04X\r\n", a);
    printf("b 0x%04X\r\n", b);
    printf("c 0x%04X\r\n", c);

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
