#include "cpu_write.h"
#include "sim.h"
#include "memory_map.h"
#include "input_device.h"
#include "output_device.h"

extern unsigned int  g_fc;
extern unsigned char g_ram[];

/* Write data to RAM or a device */
void cpu_write_byte(unsigned int address, unsigned int value)
{
  if (g_fc & 2) /* Program */
    exit_error("Attempted to write %02x to ROM address %08x", value & 0xff, address);

  /* Otherwise it's data space */
  if (address >= INPUT_ADDRESS_LO && address <= INPUT_ADDRESS_HI)
  {
    input_device_write(value & 0xff);
    return;
  }
  else if (address >= OUTPUT_ADDRESS_LO && address <= OUTPUT_ADDRESS_HI)
  {
    output_device_write(value & 0xff);
    return;
  }
  else if (address >= LED_ADDRESS_LO && address <= LED_ADDRESS_HI)
  {
    led_write(value & 0xff);
    return;
  }

  if (address > MAX_RAM)
    exit_error("Attempted to write %02x to RAM address %08x", value & 0xff, address);
  WRITE_BYTE(g_ram, address, value);
}

void cpu_write_word(unsigned int address, unsigned int value)
{
  if (g_fc & 2) /* Program */
    exit_error("Attempted to write %04x to ROM address %08x", value & 0xffff, address);

  /* Otherwise it's data space */
  if (address >= INPUT_ADDRESS_LO && address <= INPUT_ADDRESS_HI)
  {
    input_device_write(value & 0xffff);
    return;
  }
  else if (address >= OUTPUT_ADDRESS_LO && address <= OUTPUT_ADDRESS_HI)
  {
    output_device_write(value & 0xffff);
    return;
  }
  else if (address >= LED_ADDRESS_LO && address <= LED_ADDRESS_HI)
  {
    led_write(value & 0xffff);
    return;
  }

  if (address > MAX_RAM)
    exit_error("Attempted to write %04x to RAM address %08x", value & 0xffff, address);
  WRITE_WORD(g_ram, address, value);
}

void cpu_write_long(unsigned int address, unsigned int value)
{
  if (g_fc & 2) /* Program */
    exit_error("Attempted to write %08x to ROM address %08x", value, address);

  /* Otherwise it's data space */
  if (address >= INPUT_ADDRESS_LO && address <= INPUT_ADDRESS_HI)
  {
    input_device_write(value);
    return;
  }
  else if (address >= OUTPUT_ADDRESS_LO && address <= OUTPUT_ADDRESS_HI)
  {
    output_device_write(value);
    return;
  }
  else if (address >= LED_ADDRESS_LO && address <= LED_ADDRESS_HI)
  {
    led_write(value);
    return;
  }

  if (address > MAX_RAM)
    exit_error("Attempted to write %08x to RAM address %08x", value, address);
  WRITE_LONG(g_ram, address, value);
}
