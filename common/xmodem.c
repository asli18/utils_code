#include "common.h"
#include "serial.h"
#include <string.h>


#if 0
static void mdelay(U32 ms)
{
    volatile U32 c = (ms * (CPUFREQ / 1000)) / 20;
    while (c--);
}
#endif

static int _inbyte(int msec)
{
    int rc;
    while ((rc = getchar()) < 0) {
        mdelay(1);
        if (msec-- <= 0)
            return -1;
    }
    return rc;
}

static void _outbyte(unsigned char c)
{
    putchar(c);
}

static unsigned short crc16_ccitt(const unsigned char *buf, int sz)
{
    unsigned short crc = 0;
    while (--sz >= 0) {
        int i;
        crc ^= (unsigned short)*buf++ << 8;
        for (i = 0; i < 8; i++)
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc <<= 1;
    }
    return crc;
}

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A
#define DLY_1S 1000
#define MAXRETRANS 25

static int check(int crc, const unsigned char *buf, int sz)
{
    if (crc) {
        unsigned short crc = crc16_ccitt(buf, sz);
        unsigned short tcrc = (buf[sz] << 8) + buf[sz + 1];

        if (crc == tcrc)
            return 1;

    } else {
        int i;
        unsigned char cks = 0;

        for (i = 0; i < sz; ++i)
            cks += buf[i];
        if (cks == buf[sz])
            return 1;
    }
    return 0;
}

static void flushinput(void)
{
    while (_inbyte(((DLY_1S) * 3) >> 1) >= 0);
}

int32_t xmodem_receive(unsigned char *dest, uint32_t destsz)
{
    uint32_t bufsz, crc = 0;
    uint32_t i, c, len = 0;
    uint32_t retry, retrans = MAXRETRANS;
    unsigned char xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
    unsigned char *p;
    unsigned char trychar = 'C';
    unsigned char packetno = 1;


    while (1) {
        for (retry = 0; retry < 16; ++retry) {
            if (trychar) _outbyte(trychar);
            if ((c = _inbyte((DLY_1S) << 1)) >= 0) {
                switch (c) {
                case SOH:
                    bufsz = 128;
                    goto start_recv;
                case STX:
                    bufsz = 1024;
                    goto start_recv;
                case EOT:
                    flushinput();
                    _outbyte(ACK);
                    serial_printf("packetno %d\n", packetno);
                    return len; /* normal end */
                case CAN:
                    if ((c = _inbyte(DLY_1S)) == CAN) {
                        flushinput();
                        _outbyte(ACK);
                        return -1; /* canceled by remote */
                    }
                    break;
                default:
                    break;
                }
            }
        }
        if (trychar == 'C') {
            trychar = NAK;
            continue;
        }
        flushinput();
        _outbyte(CAN);
        _outbyte(CAN);
        _outbyte(CAN);
        return -2; /* sync error */
    start_recv:
        if (trychar == 'C') crc = 1;
        trychar = 0;
        p = xbuff;
        *p++ = c;
        for (i = 0; i < (bufsz + (crc ? 1 : 0) + 3); ++i) {
            if ((c = _inbyte(DLY_1S)) < 0) goto reject;
            *p++ = c;
        }
        if (xbuff[1] == (unsigned char)(~xbuff[2]) && \
            (xbuff[1] == packetno || xbuff[1] == (unsigned char)packetno - 1) && \
            check(crc, &xbuff[3], bufsz)) {
            if (xbuff[1] == packetno) {
                register uint32_t count = destsz - len;
                if (count > bufsz) count = bufsz;
                if (count > 0) {
                    memcpy (&dest[len], &xbuff[3], count);
                    len += count;
                }
                ++packetno;
                retrans = MAXRETRANS + 1;
            }
            if (--retrans <= 0) {
                flushinput();
                _outbyte(CAN);
                _outbyte(CAN);
                _outbyte(CAN);
                return -3; /* too many retry error */
            }
            _outbyte(ACK);
            continue;
        }
    reject:
        flushinput();
        _outbyte(NAK);
    }
}

