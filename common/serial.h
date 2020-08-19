#ifndef _SERIAL_H_
#define _SERIAL_H_

#define CONFIG_SERIAL_CONSOLE               (1)

#if (CONFIG_SERIAL_CONSOLE == 1)
void putchar(int c);
int getchar(void);
int _serial_printf(const char *format, ...);
int sprintf(char *out, const char *format, ...);

char *serial_next(void);
void serial_cmd_init(void);
void serial_cmd_proc(void);
void serial_cmd_add(const char *, void(*)(void));

#define serial_printf(...)                  _serial_printf(__VA_ARGS__)

#else
#define putchar(...)
#define getchar(...)                        (-1)
#define serial_printf(...)
#define sprintf(...)

#define serial_next(...)                    (NULL)
#define serial_cmd_init(...)
#define serial_cmd_proc(...)
#define serial_cmd_add(...)
#endif
#endif

