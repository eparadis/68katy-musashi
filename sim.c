#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "sim.h"
#include "m68k.h"
#include "osd.h"
#include "cpu_read.h"
#include "cpu_write.h"
#include "memory_map.h"
#include "irqs.h"
#include "nmi.h"
#include "input_device.h"
#include "output_device.h"
#include "timer.h"

void disassemble_program();

/* Data */
unsigned int flag_quit = 0; /* 1 if we want to quit */
unsigned int flag_nmi_pending = 0;  /* 1 if nmi pending */

int g_input_device_value = -1; /* Current value in input device */

// unsigned char g_rom[MAX_ROM+1];                 /* ROM */
unsigned char g_ram[MAX_RAM + 1]; /* RAM */
unsigned int g_fc;                /* Current function code from CPU */

/* Exit with an error message.  Use printf syntax. */
void exit_error(char *fmt, ...)
{
  static int guard_val = 0;
  char buff[100];
  unsigned int pc;
  va_list args;

  if (guard_val)
    return;
  else
    guard_val = 1;

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fprintf(stderr, "\n");
  pc = m68k_get_reg(NULL, M68K_REG_PPC);
  m68k_disassemble(buff, pc, M68K_CPU_TYPE_68000);
  fprintf(stderr, "At %04x: %s\n", pc, buff);

  exit(EXIT_FAILURE);
}

/* Called when the CPU pulses the RESET line */
void cpu_pulse_reset(void)
{
  nmi_device_reset();
  output_device_reset();
  input_device_reset();
}

/* Called when the CPU changes the function code pins */
void cpu_set_fc(unsigned int fc)
{
  g_fc = fc;
}

/* Parse user input and update any devices that need user input */
void get_user_input(void)
{
  static int last_ch = -1;
  int ch = osd_get_char();

  if (ch >= 0)
  {
    switch (ch)
    {
    case 0x1b:
      flag_quit = 1;
      break;
    case '~':
      if (last_ch != ch)
        flag_nmi_pending = 1;
      break;
    default:
      g_input_device_value = ch;
    }
  }
  last_ch = ch;
}

/* Disassembler */
void make_hex(char *buff, unsigned int pc, unsigned int length)
{
  char *ptr = buff;

  for (; length > 0; length -= 2)
  {
    sprintf(ptr, "%04x", cpu_read_word_dasm(pc));
    pc += 2;
    ptr += 4;
    if (length > 2)
      *ptr++ = ' ';
  }
}

void disassemble_program()
{
  unsigned int pc;
  unsigned int instr_size;
  char buff[100];
  char buff2[100];

  pc = cpu_read_long_dasm(4);

  while (pc <= 0x16e)
  {
    instr_size = m68k_disassemble(buff, pc, M68K_CPU_TYPE_68000);
    make_hex(buff2, pc, instr_size);
    printf("%03x: %-20s: %s\n", pc, buff2, buff);
    pc += instr_size;
  }
  fflush(stdout);
}

void cpu_instr_callback(int _pc)
{
  (void)_pc;
  /* The following code would print out instructions as they are executed */

  // static char buff[100];
  // static char buff2[100];
  // static unsigned int pc;
  // static unsigned int instr_size;

  // pc = m68k_get_reg(NULL, M68K_REG_PC);
  // instr_size = m68k_disassemble(buff, pc, M68K_CPU_TYPE_68000);
  // make_hex(buff2, pc, instr_size);
  // printf("E %03x: %-20s: %s\n", pc, buff2, buff);
  // fflush(stdout);
}

/* The main loop */
int main(int argc, char *argv[])
{
  FILE *fhandle;

  if (argc != 2)
  {
    printf("Usage: sim <program file>\n");
    exit(-1);
  }

  if ((fhandle = fopen(argv[1], "rb")) == NULL)
    exit_error("Unable to open %s", argv[1]);

  // if(fread(g_rom, 1, MAX_ROM+1, fhandle) <= 0)
  if (fread(g_ram, 1, MAX_RAM + 1, fhandle) <= 0)
    exit_error("Error reading %s", argv[1]);

  //	disassemble_program();

  m68k_init();
  m68k_set_cpu_type(M68K_CPU_TYPE_68000);
  m68k_pulse_reset();
  input_device_reset();
  output_device_reset();
  nmi_device_reset();

  flag_quit = 0;
  while (!flag_quit)
  {
    // Our loop requires some interleaving to allow us to update the
    // input, output, and nmi devices.

    get_user_input();

    // Values to execute determine the interleave rate.
    // Smaller values allow for more accurate interleaving with multiple
    // devices/CPUs but is more processor intensive.
    // 100000 is usually a good value to start at, then work from there.

    // Note that I am not emulating the correct clock speed!
    m68k_execute(100000);
    output_device_update();
    input_device_update();
    nmi_device_update();
    timer_update();
  }

  return 0;
}
