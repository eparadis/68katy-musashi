#ifndef SIM__HEADER
#define SIM__HEADER

// some of the musashi stuff uses 'sim.h'
// so include the definition of our IRQ handler function here
#include "irqs.h"

// uClinux reports speed (via cpuinfo) as such on my 2022 M2 MBA:
//            Clocking   BogoMips   Calibration
// 32000000 -  20.6MHz       2.57       1289600
// 16000000 -  11.9MHz       1.48        744000
//  8000000 -   7.3MHz       0.92        462400
//  4000000 -   6.5MHz       0.81        409600
//  2000000 -   7.0MHz       0.88        440000
//  1000000 -   6.8MHz       0.85        428800
#define M68K_CLOCK_SPEED 16000000 // in cycles per second

void disassemble_program();

void cpu_pulse_reset(void);
void cpu_set_fc(unsigned int fc);
void cpu_instr_callback(int pc);

void exit_error(char* fmt, ...);

void cpu_pulse_reset(void);
void cpu_set_fc(unsigned int fc);

void get_user_input(void);

void setupTerminal(void);
void resetTerminal(void);

#endif /* SIM__HEADER */
