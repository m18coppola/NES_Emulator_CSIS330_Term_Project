#include "cpu.h"

/* Reads a byte from the input address. */
unsigned char
cpu_read(CPU* cpu, unsigned short addr) 
{
	return bus_read(cpu->bus, addr);
}

/* Writes the input byte at the input address. */
void
cpu_write(CPU* cpu, unsigned short addr, unsigned char byte) 
{
	bus_write(cpu->bus, addr, byte);
}

/*
 * Cycles the clock. If there are no cycles left in the current instruction
 * it will read the next instruction in the program, setting the current 
 * opcode in the CPU.
 */
void
cpu_clock(CPU* cpu) 
{
	if (cpu->cycles == 0) {
		cpu->opcode = cpu_read(cpu, cpu->pc);
		cpu->pc++;
	
		cpu->cycles = lookup[cpu->opcode].cycles;
		
		/* Checks the address mode and the operation to see if another cycle 
		 * is needed for the instruction. Some instructions have special cases 
		 * in which another cycle is needed.
		 */
		unsigned char cycleCheck1 = (*lookup[cpu->opcode].addr_mode)(cpu);
		unsigned char cycleCheck2 = (*lookup[cpu->opcode].operate)(cpu);

		cpu->cycles += (cycleCheck1 & cycleCheck2);
	}

	cpu->cycles--;
}

/* Returns the value of the input flag in the status register. */
unsigned char 
cpu_getFlag(CPU* cpu, STATUS_FLAG f) {
	unsigned char flagCheck = cpu->status & f;
	
	/* Returns 1 if the bit that was checked is toggled on. 
	 * There are 8 different statuses so flagCheck can be greater than 1. */
	if (flagCheck > 0)
		return 1;
	
	return 0;
}

/*
 * Sets the flagged bit in the input CPU's status 
 * register with the input value. 
 */
void
cpu_setFlag(CPU* cpu, STATUS_FLAG f, bool set) {
	unsigned char status;

	status = cpu->status;
	if (set) {
		status = status | f;  
	} else {
		status = status & ~f; 
	}
}

/* 
 * Addressing Modes:
 *
 */

/* Implied */
/* 
 * Usually used for simple instructions that require no additional data.
 * Some ops will target the accumulator, so we fetch that
 */
unsigned char
IMP(CPU* cpu)
{
	cpu->fetched = cpu->a;
	return 0;
}



/* 
 *
 * Operations
 * 
 */

/* And Memory with Accumulator */
/*
 * The Accumulator register is set to the result of a bitwise and between
 * the Accumulator and the area referenced in memory.
 */
unsigned char 
AND(CPU* cpu) {
	STATUS_FLAG flagToSet;
	
	cpu->a = cpu->a & cpu->fetched;

	flagToSet = N;
	cpu_setFlag(cpu, flagToSet, true);
	flagToSet = Z;
	cpu_setFlag(cpu, flagToSet, true);

	return 1;
}

/* Clear Carry Flag */
/*
 * Clears the carry flag in the status register.
 * 
 */
unsigned char 
CLC(CPU* cpu) {
	STATUS_FLAG carryFlag;

	carryFlag = C;
	cpu_setFlag(cpu, carryFlag, false);

	return 0;
}

/* Clear Decimal Mode */
/*
 * Clears the decimal mode flag in the status register.
 * 
 */
unsigned char 
CLD(CPU* cpu) {
	STATUS_FLAG decimalFlag;

	decimalFlag = D;
	cpu_setFlag(cpu, decimalFlag, false);

	return 0;
}

/* Clear Interrupt Disable Bit */
/*
 * Clears the interrupt flag in the status register.
 *
 */
unsigned char
CLI(CPU* cpu) {
	STATUS_FLAG interruptFlag;

	interruptFlag = D;
	cpu_setFlag(cpu, interruptFlag, false);

	return 0;
}

/* Increment Y */
/*
 * Increments the Y register.flagToSet = C;
	cpu_setFlag(cpu, flagToSet, true);
 */
unsigned char 
INY(CPU* cpu) {
	STATUS_FLAG flagToSet;

	cpu->y++;

	flagToSet = N;
	cpu_setFlag(cpu, flagToSet, true);
	flagToSet = Z;
	cpu_setFlag(cpu, flagToSet, true);

	return 0;
}
