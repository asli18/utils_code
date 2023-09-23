#ifndef _COMMAND_LINE_H_
#define _COMMAND_LINE_H_

#include <config.h>

#if (CONFIG_NCURSES_EN == 1)
struct command_set {
    int (*func)(int argc, char *argv[]);
    const char *cmd_string;
    const char *help_string;
};

void cmd_proc(void);

#endif
#endif

