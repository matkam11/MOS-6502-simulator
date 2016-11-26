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