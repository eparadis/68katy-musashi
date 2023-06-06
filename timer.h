#ifndef _TIMER_H___
#define _TIMER_H___

#include <time.h>

// Timer period in clock_t units
//   clocks  |  sec
//   sec     |  100 cycles
#define TIMER_PERIOD (CLOCKS_PER_SEC/100)

#endif
