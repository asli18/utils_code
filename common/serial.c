#include "_types.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "uart.h"
#include "common.h"
#include "serial.h"


void putchar(int c)
{
    static char prev = 0;
    if (c < ' ' && c != '\r' && c != '\n' && c != '\t' && c != '\b')
        return;
    if (c == '\n' && prev != '\r')
        uart_putc(CONFIG_UART_ID, '\r');
    uart_putc(CONFIG_UART_ID, c);
    prev = c;
}

int getchar(void)
{
    return (RW_UART_REG(CONFIG_UART_ID, UART_LINE_STATUS) & CHK_UART_LINE_STATUS_DR) ? \
           (RW_UART_REG(CONFIG_UART_ID, UART_RX_PORT)) : (-1);
}

static void printchar(char **str, int c)
{
    if (str) {
        **str = c;
        ++(*str);
    } else {
        putchar(c);
    }
}

#define PAD_RIGHT (1)
#define PAD_ZERO (2)

static int prints(char **out, const char *string, int width, int pad)
{
    register int pc = 0, padchar = ' ';

    if (width > 0) {
        register int len = 0;
        register const char *ptr;
        for (ptr = string; *ptr; ++ptr) ++len;
        if (len >= width) width = 0;
        else width -= len;
        if (pad & PAD_ZERO) padchar = '0';
    }
    if (!(pad & PAD_RIGHT)) {
        for (; width > 0; --width) {
            printchar (out, padchar);
            ++pc;
        }
    }
    for (; *string ; ++string) {
        printchar (out, *string);
        ++pc;
    }
    for (; width > 0; --width) {
        printchar (out, padchar);
        ++pc;
    }

    return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN (12)

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
    char print_buf[PRINT_BUF_LEN];
    register char *s;
    register int t, neg = 0, pc = 0;
    register unsigned int u = i;

    if (i == 0) {
        print_buf[0] = '0';
        print_buf[1] = '\0';
        return prints (out, print_buf, width, pad);
    }

    if (sg && b == 10 && i < 0) {
        neg = 1;
        u = -i;
    }

    s = print_buf + PRINT_BUF_LEN - 1;
    *s = '\0';

    while (u) {
        t = u % b;
        if ( t >= 10 )
            t += letbase - '0' - 10;
        *--s = t + '0';
        u /= b;
    }

    if (neg) {
        if ( width && (pad & PAD_ZERO) ) {
            printchar (out, '-');
            ++pc;
            --width;
        } else {
            *--s = '-';
        }
    }

    return pc + prints (out, s, width, pad);
}

static int print(char **out, const char *format, va_list args)
{
    register int width, pad;
    register int pc = 0;
    char scr[2];

    for (; *format != 0; ++format) {
        if (*format == '%') {
            ++format;
            width = pad = 0;
            if (*format == '\0') break;
            if (*format == '%') goto out;
            if (*format == '-') {
                ++format;
                pad = PAD_RIGHT;
            }
            while (*format == '0') {
                ++format;
                pad |= PAD_ZERO;
            }
            for (; *format >= '0' && *format <= '9'; ++format) {
                width *= 10;
                width += *format - '0';
            }
            if (*format == 's') {
                register char *s = (char *)va_arg(args, int);
                pc += prints(out, s ? s : "(null)", width, pad);
                continue;
            }
            if (*format == 'd') {
                pc += printi (out, va_arg(args, int), 10, 1, width, pad, 'a');
                continue;
            }
            if (*format == 'x') {
                pc += printi(out, va_arg(args, int), 16, 0, width, pad, 'a');
                continue;
            }
            if (*format == 'X') {
                pc += printi(out, va_arg(args, int), 16, 0, width, pad, 'A');
                continue;
            }
            if (*format == 'u') {
                pc += printi(out, va_arg(args, int), 10, 0, width, pad, 'a');
                continue;
            }
            if (*format == 'c') {
                /* char are converted to int then pushed on the stack */
                scr[0] = (char)va_arg(args, int);
                scr[1] = '\0';
                pc += prints(out, scr, width, pad);
                continue;
            }
        } else {
        out:
            printchar(out, *format);
            ++pc;
        }
    }
    if (out) **out = '\0';
    va_end(args);
    return pc;
}

int _serial_printf(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    return print(0, format, args);
}

int sprintf(char *out, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    return print(&out, format, args);
}


#define CMD_BUF_LEN                         (24)
#define CMD_NUM_MAX                         (20)
#define CMD_PROMPT                          ("> ")
#define MAXDELIMETER                        (2)

#define SERIALCOMMANDDEBUG                  (1)
#undef SERIALCOMMANDDEBUG

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

struct serial_cmd_callback {
    char command[CMD_BUF_LEN];
    void (*function)();
};

static struct serial_cmd_callback cmd_list[CMD_NUM_MAX];
static int in_char;
static char buffer[CMD_BUF_LEN];
static int  buf_pos; // Current position in the buffer
static char *token; // Returned token from the command buffer as returned by strtok_r
static char *last; // State variable used by strtok_r during processing

// null-terminated list of character to be used as delimeters for tokenizing (default " ")
static char delim[MAXDELIMETER];

static void (*def_handler)();
static int num_cmd;


// Retrieve the next token ("word" or "argument") from the Command buffer.
// returns a NULL if no more tokens exist.
char *serial_next(void)
{
    char *nextToken;
    nextToken = strtok_r(NULL, delim, &last);
    return nextToken;
}

static void clear_buffer(void)
{
    memset((void *)buffer, 0, sizeof(buffer));
    buf_pos = 0;
}

void serial_cmd_proc(void)
{
    while (((RW_UART_REG(CONFIG_UART_ID, UART_LINE_STATUS) & CHK_UART_LINE_STATUS_DR)) > 0) {
        bool matched;

        in_char = uart_getc(CONFIG_UART_ID);

        if (in_char == ESC) {
            do {
                in_char = uart_getc(CONFIG_UART_ID);
            } while (in_char != -1);
            continue;
        }

        if ((in_char == LF) || (in_char == CR)) {
            putchar('\n');
#ifdef SERIALCOMMANDDEBUG
            serial_printf("Received: %s", buffer);
#endif

            buf_pos = 0; // Reset to start of buffer
            token = strtok_r(buffer, delim, &last); // Search for command at start of buffer

            if (token == NULL) {
                uart_puts(CONFIG_UART_ID, (U8 *)CMD_PROMPT);
                return;
            }

            matched = false;

            if (strncmp(token, "?", CMD_BUF_LEN) == 0) {

                serial_printf("commands: ");
                for (U32 new_line = 0, i = num_cmd; i >= 1;) {

                    serial_printf("%s ", cmd_list[--i].command);

                    if ((++new_line % 8) == 0)
                        serial_printf("\n");

                }
                matched = true;

            } else {

                for (U32 i = 0; i < num_cmd; ++i) {
#ifdef SERIALCOMMANDDEBUG
                    serial_printf("Comparing [%s] to [%s]\n", token, cmd_list[i].command);
#endif
                    // Compare the found command against the list of known commands for a match
                    if (strncmp(token, cmd_list[i].command, CMD_BUF_LEN) == 0) {
#ifdef SERIALCOMMANDDEBUG
                        serial_printf("Matched Command: %s\n", token);
#endif
                        while (!uart_tx_fifo_empty(CONFIG_UART_ID));

                        // Execute the stored handler function for the command
                        (*cmd_list[i].function)();
                        matched = true;
                        break;
                    }
                }
            }

            if (matched == false)
                (*def_handler)();

            putchar('\n');
            uart_puts(CONFIG_UART_ID, (U8 *)CMD_PROMPT);
            clear_buffer();

        } else if (isprint(in_char)) {
            uart_putc(CONFIG_UART_ID, in_char); // Echo back to serial stream

            /* Only printable characters into the buffer */
            if (buf_pos < (CMD_BUF_LEN - 1)) {
                buffer[buf_pos++] = in_char;  // Put character into buffer
                buffer[buf_pos] = '\0';      // Null terminate
            } else {
                serial_printf("err: serial buffer full\n");
            }

        } else {
            if (buf_pos && (in_char == BS)) { // backspace
                serial_printf("\b \b");
                buffer[--buf_pos] = '\0';
            } else {
                // not support yet
            }
        }
    }
}

// Adds a "command" and a handler function to the list of available commands.
// This is used for matching a found token in the buffer, and gives the pointer
// to the handler function to deal with it.
void serial_cmd_add(const char *command, void (*function)())
{
    if (num_cmd < CMD_NUM_MAX) {
#ifdef SERIALCOMMANDDEBUG
        serial_printf("%u-Adding command for %s\n", num_cmd, command);
#endif

        if ((strlen(command) + 1) > CMD_BUF_LEN) {
            serial_printf("warning: no enough command buf, %s %u", \
                          command, (strlen(command) + 1));
        }

        strncpy(cmd_list[num_cmd].command, command, (CMD_BUF_LEN - 1));
        cmd_list[num_cmd].command[(CMD_BUF_LEN - 1)] = '\0';
        cmd_list[num_cmd++].function = function;

    } else {
        serial_printf("err: commands list full\n");
    }
}

static void serial_cmd_add_def_handler(void (*function)(void))
{
    def_handler = function;
}

static void unrecognized(void)
{
    serial_printf("err: unrecognized commands");
}

extern void common_cmd_init(void);

void serial_cmd_init(void)
{
    strncpy(delim, " ", MAXDELIMETER);  // strtok_r needs a null-terminated string
    clear_buffer();

    common_cmd_init();

    serial_cmd_add_def_handler(unrecognized);

    uart_puts(CONFIG_UART_ID, (U8 *)CMD_PROMPT);
}

