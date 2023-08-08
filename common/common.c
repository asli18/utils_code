#include "common.h"
#include "serial.h"



void delay(U32 cycle)
{
    volatile U32 idx;

    for (idx = 0; idx < cycle; ++idx)
        NOP();
}

void udelay(U64 us)
{
    const U64 _s = get_mtime_val();

    us = us * (CPUFREQ / MHz);
    while ((get_mtime_val() - _s) < us);
}

void mdelay(U64 ms)
{
    const U64 _s = get_mtime_val();

    ms = ms * (CPUFREQ / KHz);
    while ((get_mtime_val() - _s) < ms);
}

int rand(void)
{
    static unsigned int nds_lfsr_seed = SEED;

    nds_lfsr_seed = (nds_lfsr_seed >> 1) ^ (-(nds_lfsr_seed & 1u) & 0xd0000001u);

    return (int)nds_lfsr_seed;
}

void gen_rand_pattern(void *addr, U32 len)
{
    U32 head_len = 0;
    U32 body_len = 0;
    U32 tail_len = 0;
    U32 idx;


    if (TEST_ALIGN_4((U32)addr))
        head_len = 4 - ((U32)addr & CHK_ALIGN_4);

    if (head_len > len) {
        head_len = len;
    } else {
        tail_len = (((U32)addr + len) & CHK_ALIGN_4);
        body_len = ((len - head_len - tail_len) >> 2);
    }

    for (idx = 0; idx < head_len; ++idx) {
        *(U8 *)addr = rand();
        addr = (U8 *)addr + 1;
    }

    for (idx = 0; idx < body_len; ++idx) {
        *(U32 *)addr = rand();
        addr = (U32 *)addr + 1;
    }

    for (idx = 0; idx < tail_len; ++idx) {
        *(U8 *)addr = rand();
        addr = (U8 *)addr + 1;
    }
}

void hexdump(U32 addr, U32 len)
{
    U32 idx, one_round;
    U32 end_addr;


    end_addr = addr + len;

    for (idx = addr; idx < end_addr;) {
        serial_printf("0x%08x:", idx);
        one_round = (idx + 16);

        for (; (idx < one_round) && (idx < end_addr); ++idx) {
            serial_printf(" %02x", *((U8 *)idx));
        }
        serial_printf("\n");
    }
    serial_printf("\n");
}

void hexdump_align(U32 addr, U32 len)
{
    U32 *data;
    U32 one_round;
    U32 end_addr;


    addr = SET_INC_ALIGN_4(addr);
    end_addr = addr + len;

    for (data = (U32 *)addr; (U32)data < end_addr;) {
        serial_printf("0x%08x:", (U32)data);
        one_round = (U32)(data + 4);

        for (; ((U32)data < one_round) && ((U32)data < end_addr); ++data) {
            serial_printf(" %08x", *data);
        }
        serial_printf("\n");
    }
    serial_printf("\n");
}

U32 cal_gcd(U32 a, U32 b)
{
    U32 temp;

    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

void set_bit(U32 *bit_map, U32 n)
{
    bit_map[WORD_OFFSET(n)] |= (1 << BIT_OFFSET(n));
}

void clear_bit(U32 *bit_map, U32 n)
{
    bit_map[WORD_OFFSET(n)] &= ~(1 << BIT_OFFSET(n));
}

BOOL get_bit(U32 *bit_map, U32 n)
{
    U32 bit = bit_map[WORD_OFFSET(n)] & (1 << BIT_OFFSET(n));
    return bit != 0;
}
