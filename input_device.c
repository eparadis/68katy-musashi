#include "input_device.h"
#include "irqs.h"
#include "m68k.h"
#include "nmi.h"

extern int g_input_device_value;

/* Implementation for the input device */
void input_device_reset(void)
{
  g_input_device_value = -1;
  int_controller_clear(IRQ_DATA_RDY);
}

void input_device_update(void)
{
  if (input_device_data_ready())
    int_controller_set(IRQ_DATA_RDY);
}

int input_device_ack(void)
{
  return M68K_INT_ACK_AUTOVECTOR;
}

unsigned int input_device_read(void)
{
  int value = g_input_device_value > 0 ? g_input_device_value : 0;
  int_controller_clear(IRQ_DATA_RDY);
  g_input_device_value = -1;
  return value;
}

unsigned int input_device_data_ready(void)
{
  return g_input_device_value >= 0;
}

void input_device_write(unsigned int value)
{
  (void)value;
}
