#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <test.h>
#include <command_line.h>


int test_0(void) {

    {
        uint32_t num = 1;
        char *ptr = (char *)&num;

        if (*ptr == 1) {
            PRINT("little-endian\n");
        } else {
            PRINT("big-endian\n");
        }
        PRINT("-----------------------\n");
    }
    {   /* 64bit unsigned long */
        unsigned long foo = ~0u;
        unsigned long bar = ~0ul;

        PRINT("unsigned long foo = ~0u;\n");
        PRINT("unsigned long bar = ~0ul;\n");
        PRINT("\noutput:\n");
        PRINT("foo %016lx\n", foo);
        PRINT("bar %016lx\n", bar);
        PRINT("-----------------------\n");
    }
    {
        struct rtp_header {
            unsigned cc:4;
            unsigned x:1;
            unsigned p:1;
            unsigned v:2;
            unsigned pt:7;
            unsigned m:1;
            unsigned short sequence_number;
            unsigned int timestamp;
            unsigned int ssrc;
            unsigned int csrc[0];
        } __attribute__((packed));

        struct rtp_header *header;

        PRINT("sizeof(struct rtp_header) = %lu\n", sizeof(struct rtp_header));
        PRINT("sizeof(*header) = %lu\n", sizeof(*header));
        PRINT("sizeof(header) = %lu\n", sizeof(header));
        PRINT("-----------------------\n");
    }
    { // Alignment of Structure
        struct nf_buffer1 {
            char data[10];
            int size;
            short start;
            short count;
        };
        struct nf_buffer2 {
            char data[10];
            int size;
            short start;
            short count;
        }__attribute__((packed));
        PRINT("sizeof(struct nf_buffer1) = %lu\n", sizeof(struct nf_buffer1));
        PRINT("sizeof(struct nf_buffer2) = %lu\n", sizeof(struct nf_buffer2));
        PRINT("-----------------------\n");
    }
    {
        int a[5];                  // if address of a is 0x1000
        int *p1 = (int *)(a + 1);  // p1 should be 0x1004
        int *p2 = (int *)(&a + 1); // p2 should be 0x1014
        PRINT("int a[5]; // if address of a is 0x1000\n");
        PRINT("int *p1 = (int *)(a + 1);  // p1 should be 0x1004\n");
        PRINT("int *p2 = (int *)(&a + 1); // p2 should be 0x1014\n");
        PRINT("\noutput:\n");
        PRINT("sizeof(a)=%ld, sizeof(&a)=%ld\n", sizeof(a), sizeof(&a));
        PRINT("a  0x%016lx\n", (uint64_t)a);
        PRINT("p1 0x%016lx (int *)(a + 1) = a[1]\n", (uint64_t)p1);
        PRINT("p2 0x%016lx\n", (uint64_t)p2);
        PRINT("-----------------------\n");
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

        PRINT("unsigned char s[3], mask = 0x3F;\n");
        PRINT("short a, b, c;\n");
        PRINT("s[0] = 0xE6;\n");
        PRINT("s[1] = 0xB1;\n");
        PRINT("s[2] = 0x89;\n");
        PRINT("a = (short)(s[2] & mask);                                        \n");
        PRINT("b = (short)(((s[1] & mask) << 8) | (s[2] & mask));               \n");
        PRINT("c = (short)((s[0] << 12) | ((s[1] & mask) << 6) | (s[2] & mask));\n");

        PRINT("\noutput:\n");
        PRINT("a 0x%04x\n", a);
        PRINT("b 0x%04x\n", b);
        PRINT("c 0x%04x\n", c);
    }

    return 0;
}

int test_1(void) {
    {
        int array[8] = { -1, 0, 1, 2, 3, 4, 5, 6};
        char string[8] = { -1, 0, 1, 2, 3, 4, 5, 6};
        short *data = (short *)string;
        unsigned short *data2 = (unsigned short *)array;

        PRINT("int array[8] = { -1, 0, 1, 2, 3, 4, 5, 6};\n" \
               "char string[8] = { -1, 0, 1, 2, 3, 4, 5, 6};\n" \
               "short *data = (short *)string;\n" \
               "unsigned short *data2 = (unsigned short *)array;\n\n");

        PRINT("\noutput:\n");
        PRINT("sizeof(char)        %lu\n", sizeof(char));
        PRINT("sizeof(short)       %lu\n", sizeof(short));
        PRINT("sizeof(int)         %lu\n", sizeof(int));
        PRINT("sizeof(long)        %lu\n", sizeof(long));
        PRINT("sizeof(long int)    %lu\n", sizeof(long int));
        PRINT("sizeof(long long)   %lu\n", sizeof(long long));
        PRINT("sizeof(float)       %lu\n", sizeof(float));
        PRINT("sizeof(double)      %lu\n", sizeof(double));
        PRINT("sizeof(string)      %lu\n", sizeof(string));
        PRINT("sizeof(array)       %lu\n", sizeof(array));
        PRINT("sizeof(*array)      %lu\n", sizeof(*array));
        PRINT("sizeof(string[12])  %lu\n", sizeof(string[12]));
        PRINT("sizeof(&string[0])  %lu\n", sizeof(&string[0]));
        PRINT("sizeof(data)        %lu\n", sizeof(data));
        PRINT("sizeof(*data)       %lu\n", sizeof(*data));
        PRINT("\n");
        PRINT("array  address: 0x%016lx\n", (uint64_t)&array);
        PRINT("string address: 0x%016lx\n", (uint64_t)&string);
        PRINT("data   address: 0x%016lx\n", (uint64_t)data);
        PRINT("data2  address: 0x%016lx\n", (uint64_t)data2);
        PRINT("\n");
        PRINT("string[0]      %6d\n", string[0]);
        PRINT("string[8]      %6d (out of range)\n", string[8]);
        PRINT("array[7]       %6d\n", array[7]);
        PRINT("data[0]        %6d 0x%04x\n", data[0], data[0]);
        PRINT("data[7]        %6d 0x%04x (out of range)\n", data[7], data[7]);
        PRINT("data2[0]       %6d 0x%04x\n", data2[0], data2[0]);
        PRINT("data2[3]       %6d 0x%04x\n", data2[3], data2[3]);
        PRINT("data2[4]       %6d 0x%04x\n", data2[4], data2[4]);
        PRINT("data2[5]       %6d 0x%04x\n", data2[5], data2[5]);
        PRINT("*(data2 + 9)   %6d 0x%04x\n", *(data2 + 9), *(data2 + 9));
        PRINT("*(data + 1)    %6d 0x%04x\n", *(data + 1), *(data + 1));
        PRINT("\n");

        *data = 0x1234;
        PRINT("*data = 0x1234;\n");
        PRINT("string[0]      %6d 0x%02x\n", string[0], string[0]);

    }

    return 0;
}

// type promotion
int test_2(void) {
    {
        PRINT("unsigned char a = 6;\n" \
               "char b = -20;\n" \
               "(a + b > 6) ? PRINT(\"> 6\") : PRINT(\"<= 6\");\n");

        PRINT("\noutput:\n");
        unsigned char a = 6;
        char b = -20;
        (a + b > 6) ? PRINT("> 6") : PRINT("<= 6");
    }
    PRINT("\n-----------------------\n");
    {
        PRINT("unsigned int a = 6;;\n" \
               "int b = -20;\n" \
               "(a + b > 6) ? PRINT(\"> 6\") : PRINT(\"<= 6\");\n");

        PRINT("\noutput:\n");
        unsigned int a = 6;
        int b = -20;
        (a + b > 6) ? PRINT("> 6") : PRINT("<= 6");
    }
    PRINT("\n");

    return 0;
}

// 9 x 9 Multiplication table
int test_5(void) {
    uint32_t a = 9, b = 9;

    do {
        PRINT("%u x %u = %2u\n", a, b, (a * b));

        if (--b == 0) {
            b = 9;
            a -= 1;
            PRINT("\n");
        }
    } while (a);

    return 0;
}

// MSB->LSB, LSB->MSB
int test_6(void) {
    PRINT("Reverse bits MSB->LSB LSB->MSB (32-bit)\n");
    {
        uint32_t target = 0x815a070f;
        uint32_t output = 0;

        for (uint32_t i = 0; i < 32; ++i) {
            output |= ((target >> i) & 1) << (31 - i);
        }
        PRINT("ori: 0x%08x\n", target);
        PRINT("rev: 0x%08x\n", output);
    }

    PRINT("Reverse bits MSB->LSB LSB->MSB (64-bit)\n");
    {
        uint64_t target = 0xffff7070815a070f;
        uint64_t output = 0;

        for (uint32_t i = 0; i < 64; ++i) {
            output |= ((target >> i) & 1) << (63 - i);
        }
        PRINT("ori: 0x%016lx\n", target);
        PRINT("rev: 0x%016lx\n", output);
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

    PRINT("Full pyramid of numbers:\n");

    /* Run parent loop */
    for (y = LOOP_NUM; y > 0; --y) {
        /* Print space */
        for(x = y - 1; x > 0; --x) {
            PRINT(" ");
        }

        /* Run loop to print first part of row */
        for(x = 1; x <= (LOOP_NUM - y) + 1; ++x) {
            PRINT("%d", x);
        }

        /* Run loop to print second part of row */
        for(x = (LOOP_NUM - y); x > 0; --x) {
            PRINT("%d", x);
        }

        PRINT("\n");
    }

    PRINT("\nInverted:\n");

    /* Run parent loop */
    for (y = LOOP_NUM; y > 0; --y) {
        /* Print space */
        for (x = LOOP_NUM - y; x != 0; --x) {
            PRINT(" ");
        }

        /* Run loop to print first part of row */
        for (x = 1; x <= y; ++x) {
            PRINT("%d", x);
        }

        /* Run loop to print second part of row */
        for (x = y - 1; x >= 1; --x) {
            PRINT("%d",x);
        }

        PRINT("\n");
    }

    return 0;
}

int is_prime(int num) {
    if (num == 2 || num == 3) {
        return 1;
    }
    // skip 1 and even number
    if (num == 1 || ((num & 1) == 0)) {
        return 0;
    }
    // +2 skip even number
    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

// check prime number
int test_8(void) {
#define MAX     (30)
    unsigned int count, num;

    PRINT("Prime number:\n");
    for (count = 0, num = 1; count < MAX; ++num) {
        if (is_prime(num)) {
            count += 1;
            PRINT("No.%3u  %3u\r\n", count, num);
        }
    }

    return 0;
}

int cmd_test_case(int argc, char *argv[]) {
    int (*test_ptr[])(void) = {
        test_0,
        test_1,
        test_2,
        test_3,
        test_4,
        test_5,
        test_6,
        test_7,
        test_8,
        test_9,
    };
    uint32_t bound = sizeof(test_ptr)/sizeof(*test_ptr);
    uint32_t test_index;
    int number;
    char *endptr;

    // Check if a single argument is passed and whether it's a number
    if (argc == 1 && isdigit(*argv[0])) {

        // Check if the number is in hexadecimal notation
        if (argv[0][0] == '0' && (argv[0][1] == 'x' || argv[0][1] == 'X')) {
            // Convert the hexadecimal number to an integer
            number = strtol(argv[0], &endptr, 16);
            PRINT("The number you entered is in hexadecimal notation: %u\n", number);
        } else {
            // Convert the decimal number to an integer
            number = atoi(argv[0]);
            PRINT("The number you entered is in decimal notation: %u\n", number);
        }

        test_index = (uint32_t)number;
        if (test_index < bound) {
            char test_case_str[] = "test case";

            PRINT("[ RUN      ] %s %d\n", test_case_str, test_index);
            int rc = (*test_ptr[test_index])();
            PRINT("[       OK ] %s %d\n", test_case_str, test_index);

            PRINT("[==========]\n");
            if (rc) {
                PRINT("[  FAILED  ] error code: %d\n", rc);
                return -1;
            } else {
                PRINT("[  PASSED  ]\n");
                return 0;
            }

        } else {
            PRINT("[  FAILED  ] out of test case list, range: 0 ~ %d\n", (bound - 1));
            return -1;
        }

    } else {
        PRINT("Please enter a single number(DEC/HEX) as an argument.\n");
        return -1;
    }
}

struct command_set _cmd_test_case CMD_ATTRIBUTES = {
    cmd_test_case,
    "test",
    "test [NUM]\n"
    "    Execute the specified test case.\n"
    "    0: misc, alignment of structure and bitwise operations\n"
    "    1: size of variables and pointer operations\n"
    "    2: type promotion\n"
    "    3: reverse a string\n"
    "    4: reverse words in a string\n"
    "    5: 9 x 9 multiplication table\n"
    "    6: reverse bits (32/64-bit)\n"
    "    7: print a full pyramid of numbers\n"
    "    8: check prime number\n"
    "    9: bubble sort"
};

