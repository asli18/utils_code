#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <test.h>

void test_case(uint32_t no) {
    int (*test_ptr[])(void) = {
        test_0,
        test_1,
        test_2, // type promotion
        test_3, // Reverse a string
        test_4, // Reverse words in a string
        test_5, // 9 x 9 Multiplication table
        test_6, // Reverse bits (32/64-bit)
        test_7, // print a full pyramid of numbers
        test_8, // check prime number
    };
    uint32_t bound = sizeof(test_ptr)/sizeof(*test_ptr);


    if (no < bound) {
        printf("[ RUN      ] %s %d\n", __func__, no);
        int rc = (*test_ptr[no])();
        printf("[       OK ] %s %d\n", __func__, no);

        printf("[==========]\n");
        if (rc) {
            printf("[  FAILED  ] error code: %d\n", rc);
        } else {
            printf("[  PASSED  ]\n");
        }

    } else {
        printf("[  FAILED  ] out of test case list, range: 0 ~ %d\n", (bound - 1));
    }
}

int main(int argc, char *argv[]) {
    int number;
    char *endptr;

#if 0
    number = strtol(argv[1], &endptr, 0);
    if (*endptr != '\0') {
        printf("Please enter a valid number.\n");
        return 1;
    }

    if (number >= 0) {
        printf("The number you entered is a positive number. %d\n", number);
    } else {
        printf("The number you entered is a negative number. %d\n", number);
    }
#endif

    // Check if a single argument is passed and whether it's a number
    if (argc == 2 && isdigit(*argv[1])) {

        // Check if the number is in hexadecimal notation
        if (argv[1][0] == '0' && (argv[1][1] == 'x' || argv[1][1] == 'X')) {
            // Convert the hexadecimal number to an integer
            number = strtol(argv[1], &endptr, 16);
            printf("The number you entered is in hexadecimal notation: %u\n", number);
        } else {
            // Convert the decimal number to an integer
            number = atoi(argv[1]);
            printf("The number you entered is in decimal notation: %u\n", number);

        }
        test_case((uint32_t)number);

    } else {
        printf("Please enter a single number(DEC/HEX) as an argument.\n");
    }

    return 0;
}

