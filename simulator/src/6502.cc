/*
 




*/
#include "6502.h"
#include <stdint.h>
#include <cstring>
Emulator::Emulator(uint16_t pc_start) {
	pc = pc_start;
	sr = FLAG_INTERRUPT;;
	sp = 0xFF;
	interrupt_waiting = 0x00;
	memset(mem, 0xFF, MEMORY_SIZE);
};

void Emulator::WriteMem(uint16_t address, uint8_t value) {
    mem[address] = value;
};

uint8_t Emulator::ReadMem(uint16_t address) {
	return(mem[address]);
};


/*
 * The stack in a 6502 processor is just like any other stack - values are 
 * pushed onto it and popped (“pulled” in 6502 parlance) off it. The current 
 * depth of the stack is measured by the stack pointer, a special register. 
 * The stack lives in memory between $0100 and $01ff. The stack pointer is 
 * initially $ff, which points to memory location $01ff. When a byte is pushed 
 * onto the stack, the stack pointer becomes $fe, or memory location $01fe,
 * and so on.
*/
void Emulator::StackPush(uint8_t byte) {
	Emulator::WriteMem(0x100+sp, byte);
	if(sp == 0x00) {
		sp = 0xFF;
	} else {
		sp --;
	}
};

uint8_t Emulator::StackPop() {
	if(sp == 0xFF) {
		sp = 0x00;
	} else {
		sp ++;
	}
	return Emulator::ReadMem(0x100 + sp);
};

void Emulator::Ins_jsr(uint16_t destination) {
    pc--;
    Emulator::StackPush((pc >> 8) & 0xFF);
    Emulator::StackPush(pc & 0xFF);
	pc = destination;
};

void Emulator::Ins_rts() {
	uint8_t lsb, msb;
	lsb = StackPop();
	msb = StackPop();

    // AAAAAAAAA00000000 or BBBBBBBB = AAAAAAAABBBBBBBB
    // plus 1 because return to the next instructionc
	pc = ((msb << 8 ) | lsb) + 1;
}

void Emulator::Ins_jmp(uint16_t destination) {
	pc = destination;
};

void Emulator::Ins_ldx_imm(uint8_t value) {
	x = value;
}

void Emulator::Ins_ldy_imm(uint8_t value) {
	y = value;
}

void Emulator::Ins_lda_imm(uint8_t value) {
	ac = value;
}

void Emulator::Ins_ldx_zer(uint8_t zero_addr) {
	x = Emulator::ReadMem(zero_addr);
}

void Emulator::Ins_ldy_zer(uint8_t zero_addr) {
	y = Emulator::ReadMem(zero_addr);
}

void Emulator::Ins_lda_zer(uint8_t zero_addr) {
	ac = Emulator::ReadMem(zero_addr);
}