#ifndef _TIMER_H_______
#define _TIMER_H_______

#include <time.h>

// Timer period in clock_t units
//   clocks  |  sec
//   sec     |  100 cycles
#define TIMER_PERIOD (CLOCKS_PER_SEC/100)

int timer_device_ack(void);
void timer_update(void);
void timer_device_reset(void);

#endif
