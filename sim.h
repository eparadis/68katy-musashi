#ifndef SIM__HEADER
#define SIM__HEADER

// some of the musashi stuff uses 'sim.h'
// so include the definition of our IRQ handler function here
#include "irqs.h"

void cpu_pulse_reset(void);
void cpu_set_fc(unsigned int fc);
void cpu_instr_callback(int pc);

void exit_error(char* fmt, ...);

void cpu_pulse_reset(void);
void cpu_set_fc(unsigned int fc);

void get_user_input(void);

#endif /* SIM__HEADER */
