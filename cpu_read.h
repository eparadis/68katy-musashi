#ifndef CPU_READ_H
#define CPU_READ_H

unsigned int cpu_read_byte(unsigned int address);
unsigned int cpu_read_word(unsigned int address);
unsigned int cpu_read_long(unsigned int address);

/* Read macros */
#define READ_BYTE(BASE, ADDR) (BASE)[ADDR]
#define READ_WORD(BASE, ADDR) (((BASE)[ADDR] << 8) | \
                               (BASE)[(ADDR) + 1])
#define READ_LONG(BASE, ADDR) (((BASE)[ADDR] << 24) |       \
                               ((BASE)[(ADDR) + 1] << 16) | \
                               ((BASE)[(ADDR) + 2] << 8) |  \
                               (BASE)[(ADDR) + 3])

#endif
