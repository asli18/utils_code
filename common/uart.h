#ifndef _UART_H_
#define _UART_H_

#include "_types.h"


#define UART_REG_BASE_ADDR                      (0x92000000)
#define RW_UART_REG(uart_id, offset)            (*((volatile U32 *)(UART_REG_BASE_ADDR + \
                                                 (offset) + ((uart_id) * 0x00400000))))

#define UART_IDREV                              (0x00)
#define UART_FIFO_DEPTH                         (0x10)

#define UART_OSCR                               (0x14)
#define SET_UART_OSCR_8                         (8)

#define UART_TX_PORT                            (0x20)
#define UART_RX_PORT                            (0x20)

/* DLAB == 1 */
#define UART_DLL                                (0x20)
#define UART_DLM                                (0x24)
#define UART_DL_MASK                            (BIT32_MASK(0, 8))

#define UART_INTR                               (0x24)
#define SET_UART_INTR_RX_EN                     (SET_BIT0)
#define SET_UART_INTR_TX_EN                     (SET_BIT1)
#define SET_UART_INTR_LINE_STA_EN               (SET_BIT2)

#define UART_INTR_FIFO_FLAG                     (0x28)
#define CHK_UART_INTR_TX                        (0x2)
#define CHK_UART_INTR_RX                        (0x4)
#define CHK_UART_INTR_LINE_STA                  (0x6)
#define CHK_UART_INTR_TO                        (0xc)
#define CHK_UART_FIFO_EN                        (BIT32_MASK(6, 2))

#define UART_INTR_FIFO_CONTROL                  (0x28)
#define SET_UART_FIFO_EN                        (SET_BIT0)
#define SET_UART_FIFO_RX_RESET                  (SET_BIT1)
#define SET_UART_FIFO_TX_RESET                  (SET_BIT2)
#define SET_UART_DMA_EN                         (SET_BIT3)
#define UART_INTR_TX_TRIG_LVL_OFF               (4)
#define UART_INTR_TX_TRIG_LVL_MASK              (BIT32_MASK(UART_INTR_TX_TRIG_LVL_OFF, 2))
#define UART_INTR_RX_TRIG_LVL_OFF               (6)
#define UART_INTR_RX_TRIG_LVL_MASK              (BIT32_MASK(UART_INTR_RX_TRIG_LVL_OFF, 2))

#define UART_LINE_CONTROL                       (0x2c)
#define UART_LINE_CONTROL_WLS_OFF               (0)
#define UART_LINE_CONTROL_WLS_MASK              (BIT32_MASK(UART_LINE_CONTROL_WLS_OFF, 2))
#define UART_LINE_CONTROL_WLS_5BIT              (0)
#define UART_LINE_CONTROL_WLS_6BIT              (1)
#define UART_LINE_CONTROL_WLS_7BIT              (2)
#define UART_LINE_CONTROL_WLS_8BIT              (3)
#define UART_LINE_CONTROL_STB_OFF               (2)
#define UART_LINE_CONTROL_STB_MASK              (BIT32_MASK(UART_LINE_CONTROL_STB_OFF, 1))
#define UART_LINE_CONTROL_STB_1                 (0)
#define UART_LINE_CONTROL_STB_2                 (1)
#define SET_UART_LINE_CONTROL_PARITY_EN         (SET_BIT3)
#define CLR_UART_LINE_CONTROL_PARITY_EN         (CLR_BIT3)
#define SET_UART_LINE_CONTROL_EVEN_PARITY       (SET_BIT4)
#define CLR_UART_LINE_CONTROL_EVEN_PARITY       (CLR_BIT4)
#define SET_UART_LINE_CONTROL_STICK_PARITY      (SET_BIT5)
#define CLR_UART_LINE_CONTROL_STICK_PARITY      (CLR_BIT5)
#define SET_UART_LINE_CONTROL_BC                (SET_BIT6)
#define CLR_UART_LINE_CONTROL_BC                (CLR_BIT6)
#define SET_UART_LINE_CONTROL_DLAB              (SET_BIT7)
#define CLR_UART_LINE_CONTROL_DLAB              (CLR_BIT7)

#define UART_LINE_STATUS                        (0x34)
#define CHK_UART_LINE_STATUS_DR                 (CHK_BIT0)
#define CHK_UART_LINE_STATUS_OE                 (CHK_BIT1)
#define CHK_UART_LINE_STATUS_PE                 (CHK_BIT2)
#define CHK_UART_LINE_STATUS_FE                 (CHK_BIT3)
#define CHK_UART_LINE_STATUS_LB                 (CHK_BIT4)
#define CHK_UART_LINE_STATUS_THRE               (CHK_BIT5)
#define CHK_UART_LINE_STATUS_TEMT               (CHK_BIT6)
#define CHK_UART_LINE_STATUS_ERRF               (CHK_BIT7)
#define CHK_UART_LINE_STATUS_TFNF               (CHK_BIT8) /* TX FIFO not full */

#define UART_REG_LEN                            (UART_LINE_STATUS + 4)

#define CONFIG_UART_ID                          (UART0)

#if (CONFIG_UART_ID != UART0)
#error ""
#endif

typedef enum {
    UART0,
    UART1,
} UART_ID;

typedef enum {
    UART_BR_1200,
    UART_BR_9600,
    UART_BR_38400,
    UART_BR_57600,
    UART_BR_115200,
    UART_BR_230400,
    UART_BR_460800,
    UART_BR_921600,
    UART_BR_1000000,
    UART_BR_NUM,
} UART_BR;

typedef enum {
    UART_PARITY_NONE,
    UART_PARITY_ODD,
    UART_PARITY_EVEN,
} UART_PARITY;

typedef enum {
    UART_STOP_BITS_1,
    UART_STOP_BITS_2,
} UART_STOP_BITS;


void uart_init(UART_ID id, UART_BR baud_rate, UART_PARITY parity, UART_STOP_BITS stopbits);
void uart_putc(UART_ID id, U8 c);
void uart_puts(UART_ID id, U8 *s);
int uart_getc(UART_ID id);
int uart_pollc(UART_ID id);
bool uart_tx_fifo_empty(UART_ID id);

#endif

