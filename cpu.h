enum STATUS_FLAGS {
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

