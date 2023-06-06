#ifndef _CPU_WRITE_H_
#define _CPU_WRITE_H_

void cpu_write_byte(unsigned int address, unsigned int value);
void cpu_write_word(unsigned int address, unsigned int value);
void cpu_write_long(unsigned int address, unsigned int value);

#define WRITE_BYTE(BASE, ADDR, VAL) (BASE)[ADDR] = (VAL)&0xff
#define WRITE_WORD(BASE, ADDR, VAL)   \
  (BASE)[ADDR] = ((VAL) >> 8) & 0xff; \
  (BASE)[(ADDR) + 1] = (VAL)&0xff
#define WRITE_LONG(BASE, ADDR, VAL)          \
  (BASE)[ADDR] = ((VAL) >> 24) & 0xff;       \
  (BASE)[(ADDR) + 1] = ((VAL) >> 16) & 0xff; \
  (BASE)[(ADDR) + 2] = ((VAL) >> 8) & 0xff;  \
  (BASE)[(ADDR) + 3] = (VAL)&0xff

#endif
