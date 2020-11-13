#include <stdbool.h>

/* Enumeration of flags for the status register. */
enum STATUS_FLAG {
	C = (1 << 0), /* Carry bit */
	Z = (1 << 1), /* Zero flag */
	I = (1 << 2), /* Disable interrupts */
	D = (1 << 3), /* Decimal Mode */
	B = (1 << 4), /* Break */
	U = (1 << 5), /* Unused */
	V = (1 << 6), /* Overflow */
	N = (1 << 7), /* Negative */
};

typedef struct cpu CPU;
typedef struct instruction INSTRUCTION;

/* CPU Structure */
struct cpu {
	/* Registers */
	unsigned char a; 	     /* Accumulator */
	unsigned char x; 	     /* X Register */
	unsigned char y;	     /* Y Register */
	unsigned char stkp; 	 /* Stack Pointer */
	unsigned short pc;	     /* Program Counter */
	unsigned char status; 	 /* Status Register */

	/* Bus */
	Bus* bus;

	/* Intermediate CPU States */
	unsigned char fetched;   /* Input value to ALU */
	unsigned short addr_abs; /* Current observed address */
	unsigned short addr_rel; /* Relative address */
	unsigned char opcode;    /* Current operation */
	unsigned char cycles;    /* Number of clock cycles the opcode takes */
}

/* Instruction Structure */
struct instruction {
	char* name;                     /* Name of instruction */
	unsigned char(operate)(void);   /* Pointer to operation function */
	unsigned char(addr_mode)(void); /* Pointer to addressing mode function */
	unsigned char cycles;           /* Number of cycles for instruction */
};

/* Helper functions to interact with the status register. */
unsigned cpu_getFlag(CPU* cpu, STATUS_FLAG f);
void cpu_setFlag(CPU* cpu, STATUS_FLAG f, bool v);

/* External event functions.
   These represent the physical pins entering the cpu. */
void cpu_clock(CPU* cpu); /* Performs one clock cycle. */
void cpu_reset(CPU* cpu); /* Reset interrupt. */
void cpu_irq(CPU* cpu);   /* Interrupt request. */
void cpu_nmi(CPU* cpu);   /* Non-maskable interrupt request. */

/* Fetches memory according to opcode stored in the cpu. */
void cpu_fetch(CPU* cpu);
