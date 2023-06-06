#include "timer.h"
#include "irqs.h"
#include "m68k.h"

clock_t timer_last_update = 0; /* Time of last timer update */

/* Implementation for the timer device */
void timer_update() {
  if( clock() - timer_last_update >= TIMER_PERIOD ) {
    timer_last_update = clock();
    int_controller_set(IRQ_TIMER);
  }
}

void timer_device_reset(void)
{
  timer_last_update = clock();
  int_controller_clear(IRQ_TIMER);
}

int timer_device_ack(void)
{
  int_controller_clear(IRQ_TIMER);
  return M68K_INT_ACK_AUTOVECTOR; // ???
}
