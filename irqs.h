#ifndef _IRQS_H
#define _IRQS_H

/* IRQ connections */
// the 68Katy use a 68008 which only has two interrupt pins:
// /IPL2+/IPL0 (combined into one pin) -- connected to 100 Hz oscillator
// /IPL1 -- connected to USB serial FIFO RXF: "When low, there is data available in the FIFO which can be read"
#define IRQ_TIMER 5 // signal /IP2+IP0, connected to RXF
#define IRQ_DATA_RDY 2 // signal /IP1, when active there is data to receive
#define IRQ_NMI 7 // doesn't exist on the real 68Katy

void int_controller_set(unsigned int value);
void int_controller_clear(unsigned int value);
int  cpu_irq_ack(int level);

#endif
