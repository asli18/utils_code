#ifndef _COMMON_H_
#define _COMMON_H_
#include "_types.h"


#define CHK_ALIGN_2                     (0x01ul)
#define CHK_ALIGN_4                     (0x03ul)
#define CHK_ALIGN_1K                    (0x400ul - 1)

#define TEST_ALIGN_2(a)                 (((a) & CHK_ALIGN_2) != 0)
#define TEST_ALIGN_4(a)                 (((a) & CHK_ALIGN_4) != 0)

#define SET_INC_ALIGN_2(x)              (((x) + (CHK_ALIGN_2)) & ~(CHK_ALIGN_2))
#define SET_DEC_ALIGN_2(x)              ((x) & ~(CHK_ALIGN_2))
#define SET_INC_ALIGN_4(x)              (((x) + (CHK_ALIGN_4)) & ~(CHK_ALIGN_4))
#define SET_DEC_ALIGN_4(x)              ((x) & ~(CHK_ALIGN_4))
#define SET_INC_ALIGN_1K(x)             (((x) + (CHK_ALIGN_1K)) & ~(CHK_ALIGN_1K))
#define SET_DEC_ALIGN_1K(x)             ((x) & ~(CHK_ALIGN_1K))

/* bit operation */
#define BIT32_MASK(off, len)            ((U32)((~((~0ul) << (len))) << (off)))

#define SET_BIT32(var, bit)             ((var) |= ((1ul) << (bit)))
#define CLR_BIT32(var, bit)             ((var) &= ~((1ul) << (bit)))
#define GET_BIT32(var, bit)             (((var) >> (bit)) & 1ul)

#define SWAP(a, b)                      (((a) ^ (b)) ? ((b) ^= (a) ^= (b), (a) ^= (b)) : 0)
//#define SWAP(a, b)                      (((a) ^ (b)) && ((b) ^= (a) ^= (b), (a) ^= (b)))

#define MIN(X, Y)                       (((X) < (Y)) ? (X) : (Y))

/* bitmap */
#define BITMAP_SHIFT                    (5)
#define BITMAP_MASK                     (31)
#define SET_BMAP(a, b)                  ((a)[(b) >> BITMAP_SHIFT] |= (1ul << ((b) & BITMAP_MASK)))
#define CLR_BMAP(a, b)                  ((a)[(b) >> BITMAP_SHIFT] &= ~(1ul << ((b) & BITMAP_MASK)))
#define GET_BMAP(a, b)                  (((a)[(b) >> BITMAP_SHIFT] & \
                                            (1ul << ((b) & BITMAP_MASK))) != 0)

#define WORD_OFFSET(b)                  ((b) >> (BITMAP_SHIFT))
#define BIT_OFFSET(b)                   ((b) & (BITMAP_MASK))

#define NOP() \
    ({ __asm__ __volatile__ ("nop"); })

#define WFI() \
    ({ __asm__ __volatile__ ("wfi"); })

#define LOG() \
    ({ serial_printf("%s:%u\n", __func__, __LINE__); })

#define KHz                                 (1000)
#define MHz                                 (1000000)
#define CPUFREQ                             (100 * MHz)
#define SEED                                (0x87654321)


void delay(U32 cycle);
void udelay(U64 us);
void mdelay(U64 ms);
int rand(void);
void gen_rand_pattern(void *addr, U32 len);
void hexdump(U32 addr, U32 len);
void hexdump_align(U32 addr, U32 len);
U32 cal_gcd(U32 a, U32 b);

#endif
