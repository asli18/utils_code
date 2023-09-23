#include <config.h>
#include <test.h>
#include <command_line.h>


#if (CONFIG_NCURSES_EN == 1)
int main(void) {

    cmd_proc();

    return 0;
}

#else
int main(int argc, char *argv[]) {

    cmd_test_case(argc - 1, argv + 1);

    return 0;
}
#endif

