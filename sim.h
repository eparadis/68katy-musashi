#ifndef SIM__HEADER
#define SIM__HEADER

void cpu_pulse_reset(void);
void cpu_set_fc(unsigned int fc);
void cpu_instr_callback(int pc);

void exit_error(char* fmt, ...);

void cpu_pulse_reset(void);
void cpu_set_fc(unsigned int fc);
int cpu_irq_ack(int level);

int timer_device_ack(void);
void timer_update(void);

void get_user_input(void);

#endif /* SIM__HEADER */
