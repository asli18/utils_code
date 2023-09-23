#include <string.h>
#include <ctype.h>
#include <config.h>
#include <test.h>
#include <command_line.h>


#if (CONFIG_NCURSES_EN == 1)
#define CMD_BUF_LEN                         (24)
#define CMD_PROMPT                          ("> ")
#define CMD_MAX_ARGV                        (5)

enum {
    NUL   = 0x00,
    BS    = 0x08,
    TAB   = 0x09,
    LF    = 0x0a,
    CR    = 0x0d,
    ESC   = 0x1b,
    SPACE = 0x20,
    UP    = 0x41,
    DOWN  = 0x42,
    RIGHT = 0x43,
    LEFT  = 0x44,
    DEL   = 0x7f,
};

static int cmd_display(int argc, char *argv[]);
static int cmd_help(int argc, char *argv[]);

static struct command_set cmd_list[] = {
    { cmd_display, "?",
      "?\n"
      "    Display all built-in commands in the system."
    },
    { cmd_help, "help",
      "help [command]\n"
      "    Display information about built-in commands."
    },
    { cmd_test_case, "test",
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
    },
};

static char cmd_buffer[CMD_BUF_LEN];
static int buf_pos; // Current position in the buffer

static unsigned int string_to_args(char *ptr,
                                   char *argv[],
                                   const unsigned int max_argv) {
    unsigned int n = 0;
    const char delimiter = ' ';

    if (ptr == NULL) {
        return 0;
    }

    while (*ptr == ' ') {
        ptr += 1;
    }

    while (*ptr && (n < max_argv)) {
        argv[n++] = ptr;

        while ((*ptr != delimiter) && *ptr) {
            ptr += 1;
        }

        if (*ptr == '\0') {
            break;
        }

        *ptr++ = '\0';

        while (*ptr == ' ') {
            ptr += 1;
        }
    }
    return n;
}

static int cmd_display(int argc, char *argv[]) {

    if (argc > 0) {
        PRINT("Invalid parameter\n");
        return -1;
    }

    PRINT("Command:\n");
    for (unsigned int cmd_id = 0;
         cmd_id < (sizeof(cmd_list)/sizeof(struct command_set));
         ++cmd_id) {
        PRINT("%s ", cmd_list[cmd_id].cmd_string);
    }
    PRINT("\n");
    return 0;
}

static int cmd_help(int argc, char *argv[]) {

    if (argc == 1) {
        for (unsigned int cmd_id = 0;
             cmd_id < (sizeof(cmd_list)/sizeof(struct command_set));
             ++cmd_id) {

            if (!strcmp(argv[0], cmd_list[cmd_id].cmd_string)) {
                PRINT("%s\n", cmd_list[cmd_id].info);
                return 0;
            }
        }
    }

    PRINT("Invalid parameter\n");
    return -1;
}

static void clear_buffer(void) {
    memset((void *)cmd_buffer, 0, sizeof(cmd_buffer));
    buf_pos = 0;
}

void cmd_proc_enter(void) {
    int argc;
    int rc;
    char *argv[CMD_MAX_ARGV];
    bool match;

    buf_pos = 0;

    if (0 < (argc = string_to_args(cmd_buffer, argv, CMD_MAX_ARGV))) {
        PUTCHAR('\n');
        match = false;
#if 0
        for (int id = 0; id < argc; ++id) {
            PRINT("argv[%d] %s\n", n, argv[id]);
        }
#endif

        for (unsigned int cmd_id = 0;
             cmd_id < (sizeof(cmd_list)/sizeof(struct command_set));
             ++cmd_id) {

            if (!strcmp(argv[0], cmd_list[cmd_id].cmd_string)) {
                rc = cmd_list[cmd_id].func(argc - 1, argv + 1);
                match = true;
                break;
            }
        }

        if (match == false) {
            PRINT("Error: invalid command");
        } else if (rc) {
            PRINT("Error");
        } else {
            PRINT("OK");
        }
    }

    cmd_buffer[buf_pos] = '\0';
    PUTCHAR('\n');
    PRINT(CMD_PROMPT);
}

void cmd_proc(void) {
    int in_char; // ncurses getch() returns an integer

    initscr(); // Initialize ncurses
    cbreak(); // Disable line buffering
    noecho(); // Disable echoing
    scrollok(stdscr, true); // Enable screen scrolling
    keypad(stdscr, true); // Enable special keyboard keys (e.g., arrow keys)

    clear_buffer();
    PRINT(CMD_PROMPT);

    while (1) {
        in_char = getch();

        if (in_char == ESC) {
#if 0
            do {
                in_char = getch();
            } while (in_char != -1);
            continue;
#endif
            break;
        }

        if ((in_char == LF) || (in_char == CR)) {
            cmd_proc_enter();

        } else if (isprint(in_char)) {
            PUTCHAR(in_char);

            /* Only printable characters into the buffer */
            if (buf_pos < (CMD_BUF_LEN - 1)) {
                cmd_buffer[buf_pos++] = in_char;
                cmd_buffer[buf_pos] = '\0'; // Null terminate
            } else {
                PRINT("\nError: command buffer full\n");
                buf_pos = 0;
                cmd_buffer[buf_pos] = '\0';
                PRINT(CMD_PROMPT);
                continue;
            }

        } else if (buf_pos && ((in_char == KEY_BACKSPACE) || \
                               (in_char == KEY_DC) || \
                               (in_char == BS) || \
                               (in_char == DEL))) {
            PRINT("\b \b");
            cmd_buffer[--buf_pos] = '\0';

        } else {
            // not support yet
        }
    }

    PRINT("\nPress any key to exit...");
    getch();
    endwin(); // End ncurses
}

#endif

