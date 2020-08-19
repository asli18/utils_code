#include "_types.h"
#include "uart.h"
#include "serial.h"


/* N == 10 */
#if (CONFIG_FPGA == 1)
/* default 50MHz */
static const U32 uart_clk_div[UART_BR_NUM] = {
    [UART_BR_1200]    = 2600,
    [UART_BR_9600]    = 325,
    [UART_BR_38400]   = 81,
    [UART_BR_57600]   = 54,
    [UART_BR_115200]  = 27,
    [UART_BR_230400]  = 13,
    [UART_BR_460800]  = 7,
    [UART_BR_921600]  = 3, // fail
    [UART_BR_1000000] = 3, // fail
};
#else
#if (CPUFREQ == (140 * MHz))
/* 140MHz */
static const U32 uart_clk_div[UART_BR_NUM] = {
    [UART_BR_1200]    = 7290,
    [UART_BR_9600]    = 911,
    [UART_BR_38400]   = 227,
    [UART_BR_57600]   = 151,
    [UART_BR_115200]  = 76,
    [UART_BR_230400]  = 38,
    [UART_BR_460800]  = 19,
    [UART_BR_921600]  = 9, // fail
    [UART_BR_1000000] = 9, // fail
};
#else
/* default 100MHz */
static const U32 uart_clk_div[UART_BR_NUM] = {
    [UART_BR_1200]    = 5200,
    [UART_BR_9600]    = 650,
    [UART_BR_38400]   = 162,
    [UART_BR_57600]   = 108,
    [UART_BR_115200]  = 54,
    [UART_BR_230400]  = 27,
    [UART_BR_460800]  = 13,
    [UART_BR_921600]  = 7, // fail
    [UART_BR_1000000] = 6, // fail
};
#endif
#endif


void uart_init(UART_ID id, UART_BR baud_rate, UART_PARITY parity, UART_STOP_BITS stopbits)
{
    const U16 baud_rate_divisor = 1;


    if ((id > UART1) || (baud_rate >= UART_BR_NUM) || \
        (parity > UART_PARITY_EVEN) || (stopbits > UART_STOP_BITS_2)) {
        return;
    }

    while (!(RW_UART_REG(id, UART_LINE_STATUS) & CHK_UART_LINE_STATUS_TEMT));

    RW_UART_REG(id, UART_INTR_FIFO_CONTROL) = 0;

    RW_SYS_REG(SYS_UART_CLK_DIV) = uart_clk_div[baud_rate];
    RW_UART_REG(id, UART_OSCR) = SET_UART_OSCR_8;

    RW_UART_REG(id, UART_LINE_CONTROL) |= SET_UART_LINE_CONTROL_DLAB;
    RW_UART_REG(id, UART_DLL) = (baud_rate_divisor & UART_DL_MASK);
    RW_UART_REG(id, UART_DLM) = (baud_rate_divisor >> 8) & UART_DL_MASK;
    RW_UART_REG(id, UART_LINE_CONTROL) &= CLR_UART_LINE_CONTROL_DLAB;

    RW_UART_REG(id, UART_LINE_CONTROL) =
        (UART_LINE_CONTROL_WLS_8BIT << UART_LINE_CONTROL_WLS_OFF);

    if (parity != UART_PARITY_NONE) {
        RW_UART_REG(id, UART_LINE_CONTROL) |= SET_UART_LINE_CONTROL_PARITY_EN;

        if (parity == UART_PARITY_EVEN)
            RW_UART_REG(id, UART_LINE_CONTROL) |= SET_UART_LINE_CONTROL_EVEN_PARITY;
    }

    if (stopbits == UART_STOP_BITS_2) {
        RW_UART_REG(id, UART_LINE_CONTROL) |=
            (UART_LINE_CONTROL_STB_2 << UART_LINE_CONTROL_STB_OFF);
    }

    RW_UART_REG(id, UART_INTR_FIFO_CONTROL) =
        SET_UART_FIFO_EN | SET_UART_FIFO_RX_RESET | SET_UART_FIFO_TX_RESET;

#if 0 /* output UART TX via GPIO 4 */
    RW_SYS_REG(SYS_DBG_FLAG_EN) = SET_SYS_DBG_FLAG_EN;
    RW_SYS_REG(SYS_PAD_GPIO_IN_SEL) &= ~(BIT(4) << SYS_GPIO_OUT_SEL_OFF);
    RW_SYS_REG(SYS_DBG_FLAG_MOD_SEL) =
        (SYS_DBG_FLAG_MOD_SEL_UART_00 << SYS_DBG_FLAG_MOD_H_SEL_OFF) | \
        (SYS_DBG_FLAG_MOD_SEL_UART_00 << SYS_DBG_FLAG_MOD_L_SEL_OFF);

    RW_SYS_REG(SYS_DBG_FLAG_SUB_SEL_03) = (2 << SYS_DBG_FLAG_UART0_SUB_SEL_OFF);
    serial_printf("\n\n\n");
#endif
}

void uart_putc(UART_ID id, U8 c)
{
    while (!(RW_UART_REG(id, UART_LINE_STATUS) & CHK_UART_LINE_STATUS_TFNF));
    RW_UART_REG(id, UART_TX_PORT) = c;
}

void uart_puts(UART_ID id, U8 *s)
{
    while (*s)
        uart_putc(id, *s++);
}

int uart_getc(UART_ID id)
{
    return (RW_UART_REG(id, UART_LINE_STATUS) & CHK_UART_LINE_STATUS_DR) ? \
           (RW_UART_REG(id, UART_RX_PORT)) : (-1);
}

int uart_pollc(UART_ID id)
{
    while (!(RW_UART_REG(id, UART_LINE_STATUS) & CHK_UART_LINE_STATUS_DR));
    return RW_UART_REG(id, UART_RX_PORT);
}

bool uart_tx_fifo_empty(UART_ID id)
{
    return (RW_UART_REG(id, UART_LINE_STATUS) & CHK_UART_LINE_STATUS_TEMT);
}

