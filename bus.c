#include "bus.h"

//TODO: init cpu datatype here


void
bus_clearMem(Bus* bus)
{
	int i;

	for (i = 0; i < MEM_SIZE; i++) {
		bus->ram[i] = 0x00;
	}
}

/* writes a input data into the input address 
 * write range is limited to NES's bus range*/
void
bus_write(Bus* bus, unsigned short addr, unsigned char data)
{
	/* limit writes to NES's range */
	if (addr >= 0x0000 && addr <= 0xFFFF) {
		bus->ram[addr] = data;
	}
}

/* read from the bus at input address 
 * read is limited to NES bus range 
 * if attempt to read outside of range
 * this function returns 0x00 */
unsigned char
bus_read(Bus* bus, unsigned short addr)
{
	if (addr >= 0x0000 && addr <= 0xFFFF) {
		return bus->ram[addr];
	}
	return 0x00;
}
