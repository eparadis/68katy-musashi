#include "cpu_read.h"
#include "memory_map.h"
#include "sim.h"
#include "input_device.h"
#include "output_device.h"

extern unsigned int g_output_device_ready;
extern int          g_input_device_value;
extern unsigned char g_ram[];

/* Read data from RAM, ROM, or a device */
unsigned int cpu_read_byte(unsigned int address)
{
  /* Otherwise it's data space */
  if (address >= INPUT_ADDRESS_LO && address <= INPUT_ADDRESS_HI)
  {
    return input_device_read();
  }
  else if (address >= OUTPUT_ADDRESS_LO && address <= OUTPUT_ADDRESS_HI)
  {
    return output_device_read();
  }
  else if (address >= SERIAL_STATUS_TXE_LO && address <= SERIAL_STATUS_TXE_HI)
  {
    // TXE is 0 when ready
    // g_output_Device_ready is 1 when ready
    return !g_output_device_ready;
  }
  else if (address >= SERIAL_STATUS_RDF_LO && address <= SERIAL_STATUS_RDF_HI)
  {
    // RDF is 0 when ready
    return input_device_data_ready() ? 0 : 1;
  }

  if (address > MAX_RAM)
    exit_error("Attempted to read byte from RAM address %08x", address);
  return READ_BYTE(g_ram, address);
}

unsigned int cpu_read_word(unsigned int address)
{
  /* Otherwise it's data space */
  if (address >= INPUT_ADDRESS_LO && address <= INPUT_ADDRESS_HI)
  {
    return input_device_read();
  }
  else if (address >= OUTPUT_ADDRESS_LO && address <= OUTPUT_ADDRESS_HI)
  {
    return output_device_read();
  }
  else if (address >= SERIAL_STATUS_TXE_LO && address <= SERIAL_STATUS_TXE_HI)
  {
    exit_error("attempted to read word from serial status txe address %08x", address);
  }

  if (address > MAX_RAM)
    exit_error("Attempted to read word from RAM address %08x", address);
  return READ_WORD(g_ram, address);
}

unsigned int cpu_read_long(unsigned int address)
{
  /* Otherwise it's data space */
  if (address >= INPUT_ADDRESS_LO && address <= INPUT_ADDRESS_HI)
  {
    return input_device_read();
  }
  else if (address >= OUTPUT_ADDRESS_LO && address <= OUTPUT_ADDRESS_HI)
  {
    return output_device_read();
  }
  else if (address >= SERIAL_STATUS_TXE_LO && address <= SERIAL_STATUS_TXE_HI)
  {
    exit_error("attempted to read word from serial status txe address %08x", address);
  }

  if (address > MAX_RAM)
    exit_error("Attempted to read long from RAM address %08x", address);
  return READ_LONG(g_ram, address);
}

unsigned int cpu_read_word_dasm(unsigned int address)
{
  if (address > MAX_RAM)
    exit_error("Disassembler attempted to read word from RAM address %08x", address);
  return READ_WORD(g_ram, address);
}

unsigned int cpu_read_long_dasm(unsigned int address)
{
  if (address > MAX_RAM)
    exit_error("Dasm attempted to read long from ROM address %08x", address);
  return READ_LONG(g_ram, address);
}
