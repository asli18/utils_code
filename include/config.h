#ifndef _CONFIG_H_
#define _CONFIG_H_

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
