#include "nmi.h"
#include "irqs.h"
#include <stdio.h>
#include "m68k.h"

extern unsigned int g_nmi;

/* Implementation for the NMI device */
void nmi_device_reset(void)
{
  g_nmi = 0;
}

void nmi_device_update(void)
{
  if (g_nmi)
  {
    g_nmi = 0;
    int_controller_set(IRQ_NMI);
  }
}

int nmi_device_ack(void)
{
  printf("\nNMI\n");
  fflush(stdout);
  int_controller_clear(IRQ_NMI);
  return M68K_INT_ACK_AUTOVECTOR;
}
