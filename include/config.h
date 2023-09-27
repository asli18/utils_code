#ifndef _CONFIG_H_
#define _CONFIG_H_

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__)
#define PLATFORM_ALIGNMENT          (8)
#else
#define PLATFORM_ALIGNMENT          (4)
#endif

#define CONFIG_NCURSES_EN           (1)

#if (CONFIG_NCURSES_EN == 1)
#include <ncurses.h>

#define PRINT(...)                  printw(__VA_ARGS__)
#define PUTCHAR(...)                addch(__VA_ARGS__)
#else
#define PRINT(...)                  printf(__VA_ARGS__)
#define PUTCHAR(...)                printf(__VA_ARGS__)
#endif

#endif
