#include <stdbool.h>
#include "bus.h"

/* Enumeration of flags for the status register. */
typedef enum statusFlags STATUS_FLAG;

enum statusFlags {
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
	unsigned char fetched;   /* memory fetched from address */
	unsigned short addr_abs; /* Current observed address */
	unsigned short addr_rel; /* Relative address in page */
	unsigned char opcode;    /* Current operation */
	unsigned char cycles;    /* Number of clock cycles the opcode takes */
};

/* Instruction Structure */
struct instruction {
	char* name;                     /* Name of instruction */
	unsigned char(*operate)(CPU*);   /* Pointer to operation function */
	unsigned char(*addr_mode)(CPU*); /* Pointer to addressing mode function */
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

/* lookup table for instructions */
// unimplemented functions cause linker error. will uncomment afterwards.
// for now, use null array to avoid the compiler from complaining
INSTRUCTION lookup[16 * 16];
//INSTRUCTION lookup[] = 
//{
//	{ "BRK", BRK, IMM, 7 },{ "ORA", ORA, IZX, 6 },{ "XXX", XXX, IMP, 2 },{ "XXX", XXX, IMP, 8 },{ "XXX", NOP, IMP, 3 },{ "ORA", ORA, ZP0, 3 },{ "ASL", ASL, ZP0, 5 },{ "XXX", XXX, IMP, 5 },{ "PHP", PHP, IMP, 3 },{ "ORA", ORA, IMM, 2 },{ "ASL", ASL, IMP, 2 },{ "XXX", XXX, IMP, 2 },{ "XXX", NOP, IMP, 4 },{ "ORA", ORA, ABS, 4 },{ "ASL", ASL, ABS, 6 },{ "XXX", XXX, IMP, 6 },
//	{ "BPL", BPL, REL, 2 },{ "ORA", ORA, IZY, 5 },{ "XXX", XXX, IMP, 2 },{ "XXX", XXX, IMP, 8 },{ "XXX", NOP, IMP, 4 },{ "ORA", ORA, ZPX, 4 },{ "ASL", ASL, ZPX, 6 },{ "XXX", XXX, IMP, 6 },{ "CLC", CLC, IMP, 2 },{ "ORA", ORA, ABY, 4 },{ "XXX", NOP, IMP, 2 },{ "XXX", XXX, IMP, 7 },{ "XXX", NOP, IMP, 4 },{ "ORA", ORA, ABX, 4 },{ "ASL", ASL, ABX, 7 },{ "XXX", XXX, IMP, 7 },
//	{ "JSR", JSR, ABS, 6 },{ "AND", AND, IZX, 6 },{ "XXX", XXX, IMP, 2 },{ "XXX", XXX, IMP, 8 },{ "BIT", BIT, ZP0, 3 },{ "AND", AND, ZP0, 3 },{ "ROL", ROL, ZP0, 5 },{ "XXX", XXX, IMP, 5 },{ "PLP", PLP, IMP, 4 },{ "AND", AND, IMM, 2 },{ "ROL", ROL, IMP, 2 },{ "XXX", XXX, IMP, 2 },{ "BIT", BIT, ABS, 4 },{ "AND", AND, ABS, 4 },{ "ROL", ROL, ABS, 6 },{ "XXX", XXX, IMP, 6 },
//	{ "BMI", BMI, REL, 2 },{ "AND", AND, IZY, 5 },{ "XXX", XXX, IMP, 2 },{ "XXX", XXX, IMP, 8 },{ "XXX", NOP, IMP, 4 },{ "AND", AND, ZPX, 4 },{ "ROL", ROL, ZPX, 6 },{ "XXX", XXX, IMP, 6 },{ "SEC", SEC, IMP, 2 },{ "AND", AND, ABY, 4 },{ "XXX", NOP, IMP, 2 },{ "XXX", XXX, IMP, 7 },{ "XXX", NOP, IMP, 4 },{ "AND", AND, ABX, 4 },{ "ROL", ROL, ABX, 7 },{ "XXX", XXX, IMP, 7 },
//	{ "RTI", RTI, IMP, 6 },{ "EOR", EOR, IZX, 6 },{ "XXX", XXX, IMP, 2 },{ "XXX", XXX, IMP, 8 },{ "XXX", NOP, IMP, 3 },{ "EOR", EOR, ZP0, 3 },{ "LSR", LSR, ZP0, 5 },{ "XXX", XXX, IMP, 5 },{ "PHA", PHA, IMP, 3 },{ "EOR", EOR, IMM, 2 },{ "LSR", LSR, IMP, 2 },{ "XXX", XXX, IMP, 2 },{ "JMP", JMP, ABS, 3 },{ "EOR", EOR, ABS, 4 },{ "LSR", LSR, ABS, 6 },{ "XXX", XXX, IMP, 6 },
//	{ "BVC", BVC, REL, 2 },{ "EOR", EOR, IZY, 5 },{ "XXX", XXX, IMP, 2 },{ "XXX", XXX, IMP, 8 },{ "XXX", NOP, IMP, 4 },{ "EOR", EOR, ZPX, 4 },{ "LSR", LSR, ZPX, 6 },{ "XXX", XXX, IMP, 6 },{ "CLI", CLI, IMP, 2 },{ "EOR", EOR, ABY, 4 },{ "XXX", NOP, IMP, 2 },{ "XXX", XXX, IMP, 7 },{ "XXX", NOP, IMP, 4 },{ "EOR", EOR, ABX, 4 },{ "LSR", LSR, ABX, 7 },{ "XXX", XXX, IMP, 7 },
//	{ "RTS", RTS, IMP, 6 },{ "ADC", ADC, IZX, 6 },{ "XXX", XXX, IMP, 2 },{ "XXX", XXX, IMP, 8 },{ "XXX", NOP, IMP, 3 },{ "ADC", ADC, ZP0, 3 },{ "ROR", ROR, ZP0, 5 },{ "XXX", XXX, IMP, 5 },{ "PLA", PLA, IMP, 4 },{ "ADC", ADC, IMM, 2 },{ "ROR", ROR, IMP, 2 },{ "XXX", XXX, IMP, 2 },{ "JMP", JMP, IND, 5 },{ "ADC", ADC, ABS, 4 },{ "ROR", ROR, ABS, 6 },{ "XXX", XXX, IMP, 6 },
//	{ "BVS", BVS, REL, 2 },{ "ADC", ADC, IZY, 5 },{ "XXX", XXX, IMP, 2 },{ "XXX", XXX, IMP, 8 },{ "XXX", NOP, IMP, 4 },{ "ADC", ADC, ZPX, 4 },{ "ROR", ROR, ZPX, 6 },{ "XXX", XXX, IMP, 6 },{ "SEI", SEI, IMP, 2 },{ "ADC", ADC, ABY, 4 },{ "XXX", NOP, IMP, 2 },{ "XXX", XXX, IMP, 7 },{ "XXX", NOP, IMP, 4 },{ "ADC", ADC, ABX, 4 },{ "ROR", ROR, ABX, 7 },{ "XXX", XXX, IMP, 7 },
//	{ "XXX", NOP, IMP, 2 },{ "STA", STA, IZX, 6 },{ "XXX", NOP, IMP, 2 },{ "XXX", XXX, IMP, 6 },{ "STY", STY, ZP0, 3 },{ "STA", STA, ZP0, 3 },{ "STX", STX, ZP0, 3 },{ "XXX", XXX, IMP, 3 },{ "DEY", DEY, IMP, 2 },{ "XXX", NOP, IMP, 2 },{ "TXA", TXA, IMP, 2 },{ "XXX", XXX, IMP, 2 },{ "STY", STY, ABS, 4 },{ "STA", STA, ABS, 4 },{ "STX", STX, ABS, 4 },{ "XXX", XXX, IMP, 4 },
//	{ "BCC", BCC, REL, 2 },{ "STA", STA, IZY, 6 },{ "XXX", XXX, IMP, 2 },{ "XXX", XXX, IMP, 6 },{ "STY", STY, ZPX, 4 },{ "STA", STA, ZPX, 4 },{ "STX", STX, ZPY, 4 },{ "XXX", XXX, IMP, 4 },{ "TYA", TYA, IMP, 2 },{ "STA", STA, ABY, 5 },{ "TXS", TXS, IMP, 2 },{ "XXX", XXX, IMP, 5 },{ "XXX", NOP, IMP, 5 },{ "STA", STA, ABX, 5 },{ "XXX", XXX, IMP, 5 },{ "XXX", XXX, IMP, 5 },
//	{ "LDY", LDY, IMM, 2 },{ "LDA", LDA, IZX, 6 },{ "LDX", LDX, IMM, 2 },{ "XXX", XXX, IMP, 6 },{ "LDY", LDY, ZP0, 3 },{ "LDA", LDA, ZP0, 3 },{ "LDX", LDX, ZP0, 3 },{ "XXX", XXX, IMP, 3 },{ "TAY", TAY, IMP, 2 },{ "LDA", LDA, IMM, 2 },{ "TAX", TAX, IMP, 2 },{ "XXX", XXX, IMP, 2 },{ "LDY", LDY, ABS, 4 },{ "LDA", LDA, ABS, 4 },{ "LDX", LDX, ABS, 4 },{ "XXX", XXX, IMP, 4 },
//	{ "BCS", BCS, REL, 2 },{ "LDA", LDA, IZY, 5 },{ "XXX", XXX, IMP, 2 },{ "XXX", XXX, IMP, 5 },{ "LDY", LDY, ZPX, 4 },{ "LDA", LDA, ZPX, 4 },{ "LDX", LDX, ZPY, 4 },{ "XXX", XXX, IMP, 4 },{ "CLV", CLV, IMP, 2 },{ "LDA", LDA, ABY, 4 },{ "TSX", TSX, IMP, 2 },{ "XXX", XXX, IMP, 4 },{ "LDY", LDY, ABX, 4 },{ "LDA", LDA, ABX, 4 },{ "LDX", LDX, ABY, 4 },{ "XXX", XXX, IMP, 4 },
//	{ "CPY", CPY, IMM, 2 },{ "CMP", CMP, IZX, 6 },{ "XXX", NOP, IMP, 2 },{ "XXX", XXX, IMP, 8 },{ "CPY", CPY, ZP0, 3 },{ "CMP", CMP, ZP0, 3 },{ "DEC", DEC, ZP0, 5 },{ "XXX", XXX, IMP, 5 },{ "INY", INY, IMP, 2 },{ "CMP", CMP, IMM, 2 },{ "DEX", DEX, IMP, 2 },{ "XXX", XXX, IMP, 2 },{ "CPY", CPY, ABS, 4 },{ "CMP", CMP, ABS, 4 },{ "DEC", DEC, ABS, 6 },{ "XXX", XXX, IMP, 6 },
//	{ "BNE", BNE, REL, 2 },{ "CMP", CMP, IZY, 5 },{ "XXX", XXX, IMP, 2 },{ "XXX", XXX, IMP, 8 },{ "XXX", NOP, IMP, 4 },{ "CMP", CMP, ZPX, 4 },{ "DEC", DEC, ZPX, 6 },{ "XXX", XXX, IMP, 6 },{ "CLD", CLD, IMP, 2 },{ "CMP", CMP, ABY, 4 },{ "NOP", NOP, IMP, 2 },{ "XXX", XXX, IMP, 7 },{ "XXX", NOP, IMP, 4 },{ "CMP", CMP, ABX, 4 },{ "DEC", DEC, ABX, 7 },{ "XXX", XXX, IMP, 7 },
//	{ "CPX", CPX, IMM, 2 },{ "SBC", SBC, IZX, 6 },{ "XXX", NOP, IMP, 2 },{ "XXX", XXX, IMP, 8 },{ "CPX", CPX, ZP0, 3 },{ "SBC", SBC, ZP0, 3 },{ "INC", INC, ZP0, 5 },{ "XXX", XXX, IMP, 5 },{ "INX", INX, IMP, 2 },{ "SBC", SBC, IMM, 2 },{ "NOP", NOP, IMP, 2 },{ "XXX", SBC, IMP, 2 },{ "CPX", CPX, ABS, 4 },{ "SBC", SBC, ABS, 4 },{ "INC", INC, ABS, 6 },{ "XXX", XXX, IMP, 6 },
//	{ "BEQ", BEQ, REL, 2 },{ "SBC", SBC, IZY, 5 },{ "XXX", XXX, IMP, 2 },{ "XXX", XXX, IMP, 8 },{ "XXX", NOP, IMP, 4 },{ "SBC", SBC, ZPX, 4 },{ "INC", INC, ZPX, 6 },{ "XXX", XXX, IMP, 6 },{ "SED", SED, IMP, 2 },{ "SBC", SBC, ABY, 4 },{ "NOP", NOP, IMP, 2 },{ "XXX", XXX, IMP, 7 },{ "XXX", NOP, IMP, 4 },{ "SBC", SBC, ABX, 4 },{ "INC", INC, ABX, 7 },{ "XXX", XXX, IMP, 7 },
//};

