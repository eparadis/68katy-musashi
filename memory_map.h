#ifndef MEMORY_MAP__HEADER
#define MEMORY_MAP__HEADER

/* Memory-mapped IO ports */
#define INPUT_ADDRESS_LO 0x78000
#define INPUT_ADDRESS_HI 0x79fff
#define OUTPUT_ADDRESS_LO 0x7a000
#define OUTPUT_ADDRESS_HI 0x7bfff
#define LED_ADDRESS_LO 0x7e000
#define LED_ADDRESS_HI 0x7ffff
#define SERIAL_STATUS_TXE_LO 0x7D000
#define SERIAL_STATUS_TXE_HI 0x7DFFF
#define SERIAL_STATUS_RDF_LO 0x7c000
#define SERIAL_STATUS_RDF_HI 0x7cfff

/* ROM and RAM sizes */
// #define MAX_ROM 0x77fff
#define MAX_ROM 0x0 // no ROM
#define MAX_RAM 0xfffff

#endif /* MEMORY_MAP__HEADER */
