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

void
cpu_fetch(CPU* cpu)
{
	if (lookup[cpu->opcode].addr_mode != IMP) {
		cpu->fetched = cpu_read(cpu, cpu->addr_abs);
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
 * 1) fetch memory
 * 2) implement opcode
 * 3) set flags
 * 4) return the possibility of extra clock cycles
 *
 */

/* Add Memory to A with Carry */
/*
 * Adds the fetched memory to the Accumulator register and the carry bit.
 */
unsigned char 
ADC(CPU* cpu) {
	cpu_fetch(cpu);

	unsigned char result = cpu->a + cpu->fetched + cpu_getFlag(cpu, C);

	cpu_setFlag(cpu, V, (cpu->a & 0x80) != (result & 0x80));
	cpu_setFlag(cpu, N, cpu->a & 0x80);
	cpu_setFlag(cpu, Z, result == 0X00);

	// TODO - Implement the proper functionality for ADC regarding overflow.
	// Not finished.

	return 1;
}

/* And Memory with Accumulator */
/*
 * The Accumulator register is set to the result of a bitwise and between
 * the Accumulator and the area referenced in memory.
 */
unsigned char 
AND(CPU* cpu) {
	cpu_fetch(cpu);
	cpu->a = cpu->a & cpu->fetched;
	cpu_setFlag(cpu, N, cpu->a & 0x80);
	cpu_setFlag(cpu, Z, cpu->a == 0x00);
	return 1;
}

/* Arithmetic Shift Left */
/*
 * Shifts the referenced byte (fetched memory or register) one bit.
 */
unsigned char 
ASL(CPU* cpu) {
	cpu_fetch(cpu);

	cpu_setFlag(cpu, C, cpu->fetched & 0x80);

	cpu->fetched = (cpu->fetched << 1) & 0xFE;
	
	cpu_setFlag(cpu, N, cpu->fetched & 0x80);
	cpu_setFlag(cpu, Z, cpu->fetched == 0x00);

	return 0;
}

/* Clear Carry Flag */
/*
 * Clears the carry flag in the status register.
 */
unsigned char 
CLC(CPU* cpu) {
	cpu_setFlag(cpu, C, false);

	return 0;
}

/* Clear Decimal Mode */
/*
 * Clears the decimal mode flag in the status register.
 */
unsigned char 
CLD(CPU* cpu) {
	cpu_setFlag(cpu, D, false);

	return 0;
}

/* Clear Interrupt Disable Bit */
/*
 * Clears the interrupt flag in the status register.
 */
unsigned char
CLI(CPU* cpu) {
	cpu_setFlag(cpu, I, false);

	return 0;
}

/* Clear Overflow Flag */
/*
 * Clears the overflow flag in the status register.
 */
unsigned char
CLV(CPU* cpu) {
	cpu_setFlag(cpu, V, false);

	return 0;
}

/* Compare A with Memory */
/*
 * Compares the Accumulator register with the fetched memory.
 */
unsigned char
CMP(CPU* cpu) {
	cpu_fetch(cpu);

	unsigned char result = cpu->a - cpu->fetched;

	cpu_setFlag(cpu, N, result & 0x80);
	cpu_setFlag(cpu, C, cpu->a >= cpu->fetched);
	cpu_setFlag(cpu, Z, result == 0x00);

	return 1;
}

/* Compare X with Memory */
/*
 * Compares the X register with the fetched memory.
 */
unsigned char
CPX(CPU* cpu) {
	cpu_fetch(cpu);

	unsigned char result = cpu->x - cpu->fetched;

	cpu_setFlag(cpu, N, result & 0x80);
	cpu_setFlag(cpu, C, cpu->x >= cpu->fetched);
	cpu_setFlag(cpu, Z, result == 0x00);

	return 0;
}

/* Compare Y with Memory */
/*
 * Compares the Y register with the fetched memory.
 */
unsigned char
CPY(CPU* cpu) {
	cpu_fetch(cpu);

	unsigned char result = cpu->y - cpu->fetched;

	cpu_setFlag(cpu, N, result & 0x80);
	cpu_setFlag(cpu, C, cpu->y >= cpu->fetched);
	cpu_setFlag(cpu, Z, result == 0x00);

	return 0;
}

/* Decrement Memory by one */
/*
 * Decrements the fetched memory by one.
 */
unsigned char
DEC(CPU* cpu) {
	cpu_fetch(cpu);

	cpu->fetched = (cpu->fetched - 1) & 0xFF;

	cpu_setFlag(cpu, N, cpu->fetched & 0x80);
	cpu_setFlag(cpu, Z, cpu->fetched == 0x00);

	return 0;
}

/* Decrement X by one */
/*
 * Decrements the X register by one.
 */
unsigned char
DEX(CPU* cpu) {
	cpu->x = (cpu->x - 1);

	cpu_setFlag(cpu, N, cpu->x & 0x80);
	cpu_setFlag(cpu, Z, cpu->x == 0x00);

	return 0;
}

/* Decrement Y by one */
/*
 * Decrements the Y register by one.
 */
unsigned char
DEY(CPU* cpu) {
	cpu->y = (cpu->y - 1);

	cpu_setFlag(cpu, N, cpu->y & 0x80);
	cpu_setFlag(cpu, Z, cpu->y == 0x00);

	return 0;
}

/* Bitwise XOR A with Memory */
/*
 * The Accumulator register is set to the result of a bitwise
 * exclusive or with the Accumulator register and the the fetched memory.
 */
unsigned char
EOR(CPU* cpu) {
	cpu->a = cpu->a ^ cpu->fetched;

	cpu_setFlag(cpu, N, cpu->a & 0x80);
	cpu_setFlag(cpu, Z, cpu->a == 0x00);

	return 1;
}

/* Increment Memory by one */
/*
 * Increments the fetched memory by one.
 */
unsigned char
INC(CPU* cpu) {
	cpu_fetch(cpu);

	cpu->fetched = (cpu->fetched + 1) & 0xFF;

	cpu_setFlag(cpu, N, cpu->fetched & 0x80);
	cpu_setFlag(cpu, Z, cpu->fetched == 0x00);

	return 0;
}

/* Increment X */
/*
 * Increments the X register by one.
 */
unsigned char 
INX(CPU* cpu) {
	cpu->x = cpu->x + 1;

	cpu_setFlag(cpu, N, cpu->x & 0x80);
	cpu_setFlag(cpu, Z, cpu->x == 0x00);

	return 0;
}

/* Increment Y */
/*
 * Increments the Y register by one.
 */
unsigned char 
INY(CPU* cpu) {
	cpu->y = cpu->y + 1;

	cpu_setFlag(cpu, N, cpu->y & 0x80);
	cpu_setFlag(cpu, Z, cpu->y == 0x00);

	return 0;
}
