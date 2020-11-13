#include <stdbool.h>
#include "bus.h"

/* Enumeration of flags for the status register. */
enum STATUS_FLAG {
	C = (1 << 0),   /* Carry bit */
	Z = (1 << 1),   /* Zero flag */
	I = (1 << 2),   /* Disable interrupts */
	D = (1 << 3),   /* Decimal Mode */
	B = (1 << 4),   /* Break */
	U = (1 << 5),   /* Unused */
	V = (1 << 6),   /* Overflow */
	N = (1 << 7),   /* Negative */
};

typedef struct cpu CPU;
typedef struct instruction INSTRUCTION;

/* CPU Structure */
struct cpu {
	/* Registers */
	unsigned char a;       /* Accumulator */
	unsigned char x;       /* X Register */
	unsigned char y;       /* Y Register */
	unsigned char stkp;    /* Stack Pointer */
	unsigned short pc;     /* Program Counter */
	unsigned char status;  /* Status Register */

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
unsigned char cpu_getFlag(CPU* cpu, STATUS_FLAG f);
void cpu_setFlag(CPU* cpu, STATUS_FLAG f, bool set);

/*
 * External event functions.
 * These represent the physical pins entering the cpu.
 */
void cpu_clock(CPU* cpu);   /* Performs one clock cycle. */
void cpu_reset(CPU* cpu);   /* Reset interrupt. */
void cpu_irq(CPU* cpu);     /* Interrupt request. */
void cpu_nmi(CPU* cpu);     /* Non-maskable interrupt request. */

/* Fetches memory according to opcode stored in the cpu. */
void cpu_fetch(CPU* cpu);

/* 6502 Addressing Modes */
unsigned char IMP(CPU* cpu);  unsigned char IMM(CPU* cpu);	
unsigned char ZP0(CPU* cpu);  unsigned char ZPX(CPU* cpu);	
unsigned char ZPY(CPU* cpu);  unsigned char REL(CPU* cpu);
unsigned char ABS(CPU* cpu);  unsigned char ABX(CPU* cpu);	
unsigned char ABY(CPU* cpu);  unsigned char IND(CPU* cpu);	
unsigned char IZX(CPU* cpu);  unsigned char IZY(CPU* cpu);

/* 6506 Instruction Set */
unsigned char ADC(CPU* cpu);	unsigned char AND(CPU* cpu);	unsigned char ASL(CPU* cpu);	unsigned char BCC(CPU* cpu);
unsigned char BCS(CPU* cpu);	unsigned char BEQ(CPU* cpu);	unsigned char BIT(CPU* cpu);	unsigned char BMI(CPU* cpu);
unsigned char BNE(CPU* cpu);	unsigned char BPL(CPU* cpu);	unsigned char BRK(CPU* cpu);	unsigned char BVC(CPU* cpu);
unsigned char BVS(CPU* cpu);	unsigned char CLC(CPU* cpu);	unsigned char CLD(CPU* cpu);	unsigned char CLI(CPU* cpu);
unsigned char CLV(CPU* cpu);	unsigned char CMP(CPU* cpu);	unsigned char CPX(CPU* cpu);	unsigned char CPY(CPU* cpu);
unsigned char DEC(CPU* cpu);	unsigned char DEX(CPU* cpu);	unsigned char DEY(CPU* cpu);	unsigned char EOR(CPU* cpu);
unsigned char INC(CPU* cpu);	unsigned char INX(CPU* cpu);	unsigned char INY(CPU* cpu);	unsigned char JMP(CPU* cpu);
unsigned char JSR(CPU* cpu);	unsigned char LDA(CPU* cpu);	unsigned char LDX(CPU* cpu);	unsigned char LDY(CPU* cpu);
unsigned char LSR(CPU* cpu);	unsigned char NOP(CPU* cpu);	unsigned char ORA(CPU* cpu);	unsigned char PHA(CPU* cpu);
unsigned char PHP(CPU* cpu);	unsigned char PLA(CPU* cpu);	unsigned char PLP(CPU* cpu);	unsigned char ROL(CPU* cpu);
unsigned char ROR(CPU* cpu);	unsigned char RTI(CPU* cpu);	unsigned char RTS(CPU* cpu);	unsigned char SBC(CPU* cpu);
unsigned char SEC(CPU* cpu);	unsigned char SED(CPU* cpu);	unsigned char SEI(CPU* cpu);	unsigned char STA(CPU* cpu);
unsigned char STX(CPU* cpu);	unsigned char STY(CPU* cpu);	unsigned char TAX(CPU* cpu);	unsigned char TAY(CPU* cpu);
unsigned char TSX(CPU* cpu);	unsigned char TXA(CPU* cpu);	unsigned char TXS(CPU* cpu);	unsigned char TYA(CPU* cpu);


/*
 * Illegal operation capture (undefined opcodes)
 * works as a NOP instruction
 */
unsigned char XXX(CPU* cpu);
