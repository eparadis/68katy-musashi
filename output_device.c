#include "output_device.h"
#include "m68k.h"
#include <stdio.h>

unsigned int g_output_device_ready = 0; /* 1 if output device is ready */
clock_t g_output_device_last_output;    /* Time of last char output */

/* Implementation for the output device */
void output_device_reset(void)
{
  g_output_device_last_output = clock();
  g_output_device_ready = 0;
  // int_controller_clear(IRQ_OUTPUT_DEVICE);
}

void output_device_update(void)
{
  if (!g_output_device_ready)
  {
    if ((clock() - g_output_device_last_output) >= OUTPUT_DEVICE_PERIOD)
    {
      g_output_device_ready = 1;
      // int_controller_set(IRQ_OUTPUT_DEVICE);
    }
  }
}

int output_device_ack(void)
{
  return M68K_INT_ACK_AUTOVECTOR;
}

unsigned int output_device_read(void)
{
  // int_controller_clear(IRQ_OUTPUT_DEVICE);
  return 0;
}

void output_device_write(unsigned int value)
{
  char ch;
  if (g_output_device_ready)
  {
    ch = value & 0xff;
    printf("%c", ch); fflush(stdout);
    g_output_device_last_output = clock();
    g_output_device_ready = 0;
    // int_controller_clear(IRQ_OUTPUT_DEVICE);
  }
}

void led_write(unsigned int value)
{
  (void)value;
  // printf("LED: %08b\n", value);
}
