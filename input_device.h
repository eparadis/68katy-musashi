#ifndef _INPUT_DEVICE_H
#define _INPUT_DEVICE_H

void input_device_reset(void);
void input_device_update(void);
int input_device_ack(void);
unsigned int input_device_read(void);
void input_device_write(unsigned int value);
unsigned int input_device_data_ready(void);

#endif
