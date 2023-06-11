#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
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

/* Data */
unsigned int flag_quit = 0; /* 1 if we want to quit */
unsigned int flag_nmi_pending = 0;  /* 1 if nmi pending */

int g_input_device_value = -1; /* Current value in input device */

// unsigned char g_rom[MAX_ROM+1];                 /* ROM */
unsigned char g_ram[MAX_RAM + 1]; /* RAM */
unsigned int g_fc;                /* Current function code from CPU */

struct termios oldtio, newtio;

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

  resetTerminal();
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
  static int tilde_count = 0;
  int ch = osd_get_char();

  if (ch >= 0)
  {
    switch (ch)
    {
    case '~':
      // press '~' three times to quit
      tilde_count += 1;
      if( tilde_count >= 3) {
        flag_quit = 1;
        tilde_count = 0;
      }
      // otherwise press '~' to cause an NMI
      if (last_ch != ch)
        flag_nmi_pending = 1;
      break;
    default:
      g_input_device_value = ch;
      tilde_count = 0;
    }
  }
  last_ch = ch;
}

void setupTerminal() {
  // largely from pforth pf_io_posix.c
  tcgetattr(STDIN_FILENO, &oldtio);
  tcgetattr(STDIN_FILENO, &newtio);
  newtio.c_lflag &= ~( ECHO | ECHONL | ECHOCTL | ICANON );
  newtio.c_cc[VTIME] = 0;
  newtio.c_cc[VMIN] = 1;
  //cfmakeraw(&newtio); // use this instead of the three lines above if you want the VM to be able to output CR and LF independently
  if( tcsetattr(STDIN_FILENO, TCSANOW, &newtio) < 0 ) {
    printf("error setting terminal");
  }

  // remove buffers from stdin & stdout 
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);

  // ignore SIGINT
  signal(SIGINT, SIG_IGN);
}

void resetTerminal() {
  tcsetattr(STDIN_FILENO, TCSANOW, &oldtio);
  signal(SIGINT, SIG_DFL);
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
  clock_t prev_loop;

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

  setupTerminal();
  m68k_init();
  m68k_set_cpu_type(M68K_CPU_TYPE_68000);
  m68k_pulse_reset();
  timer_device_reset();

  flag_quit = 0;
  while (!flag_quit)
  {
    // interleave running the cpu and the devices

    // use clock() to find out how long it's been since we executed cycles
    // then execute the number of cycles that should have run in that time for our processor speed
    m68k_execute((clock() - prev_loop) * M68K_CLOCK_SPEED / CLOCKS_PER_SEC);
    prev_loop = clock();

    // handle terminal input
    get_user_input();

    // update all the emulated devices.
    // they handle their own state, raising IRQs, etc
    output_device_update();
    input_device_update();
    nmi_device_update();
    timer_update();
  }

  resetTerminal();
  return 0;
}
