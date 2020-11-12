#define MEM_SIZE 64 * 1024

typedef struct bus Bus;

struct bus {
	unsigned char ram[MEM_SIZE];
};

void bus_clearMem(Bus* bus);
void bus_write(Bus* bus, unsigned short addr, unsigned char data);
unsigned char bus_read(Bus* bus, unsigned short addr);
