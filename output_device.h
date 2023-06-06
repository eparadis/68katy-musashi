#ifndef _OUTPUT_DEVICE_H
#define _OUTPUT_DEVICE_H

#include <time.h>

/* Time between characters sent to output device (host clock ticks) */
#define OUTPUT_DEVICE_PERIOD (CLOCKS_PER_SEC/9600)

void output_device_reset(void);
void output_device_update(void);
int output_device_ack(void);
unsigned int output_device_read(void);
void output_device_write(unsigned int value);

void led_write(unsigned int value);

#endif
