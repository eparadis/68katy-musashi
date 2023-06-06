#ifndef SIM__HEADER
#define SIM__HEADER

void cpu_pulse_reset(void);
void cpu_set_fc(unsigned int fc);
int  cpu_irq_ack(int level);
void cpu_instr_callback(int pc);

void exit_error(char* fmt, ...);

void cpu_pulse_reset(void);
void cpu_set_fc(unsigned int fc);
int cpu_irq_ack(int level);

void input_device_reset(void);
void input_device_update(void);
int input_device_ack(void);
unsigned int input_device_read(void);
void input_device_write(unsigned int value);

void output_device_reset(void);
void output_device_update(void);
int output_device_ack(void);
unsigned int output_device_read(void);
void output_device_write(unsigned int value);

void led_write(unsigned int value);

int timer_device_ack(void);
void timer_update(void);

void int_controller_set(unsigned int value);
void int_controller_clear(unsigned int value);

void get_user_input(void);

#endif /* SIM__HEADER */
