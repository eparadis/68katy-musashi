#include "irqs.h"
#include "m68k.h"
#include "nmi.h"
#include "timer.h"
#include "input_device.h"

unsigned int irq_controller_pending = 0;     /* list of pending interrupts */
unsigned int irq_controller_highest_int = 0; /* Highest pending interrupt */

/* Called when the CPU acknowledges an interrupt */
int cpu_irq_ack(int level)
{
  switch (level)
  {
  case IRQ_NMI:
    return nmi_device_ack();
  // case IRQ_INPUT_DEVICE:
  // return input_device_ack();
  // case IRQ_OUTPUT_DEVICE:
  // return output_device_ack();
  case IRQ_DATA_RDY:
    return input_device_ack();
  case IRQ_TIMER:
    return timer_device_ack();
  }
  return M68K_INT_ACK_SPURIOUS;
}

/* Implementation for the interrupt controller */
void int_controller_set(unsigned int value)
{
  unsigned int old_pending = irq_controller_pending;

  irq_controller_pending |= (1 << value);

  if (old_pending != irq_controller_pending && value > irq_controller_highest_int)
  {
    irq_controller_highest_int = value;
    m68k_set_irq(irq_controller_highest_int);
  }
}

void int_controller_clear(unsigned int value)
{
  irq_controller_pending &= ~(1 << value);

  for (irq_controller_highest_int = 7; irq_controller_highest_int > 0; irq_controller_highest_int--)
    if (irq_controller_pending & (1 << irq_controller_highest_int))
      break;

  m68k_set_irq(irq_controller_highest_int);
}

