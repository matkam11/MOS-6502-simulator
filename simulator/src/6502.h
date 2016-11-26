/*
 




*/
#define MEMORY_SIZE 65536
#define STACK_START 0x0100

#define FLAG_NEGATIVE 0x80
#define FLAG_OVERFLOW 0x40
#define FLAG_BREAK 0x10
#define FLAG_DECIMAL 0x08
#define FLAG_INTERRUPT 0x04
#define FLAG_ZERO 0x02
#define FLAG_CARRY 0x01

#include <stdint.h>
// set if memory was modified during processing of the last instruction
#define EMU_FLAG_DIRTY 0x01
// set if the emulator should wait for an interrupt before continuing
#define EMU_FLAG_WAIT_FOR_INTERRUPT 0x02
class Emulator {
   public:
    Emulator(uint16_t pc_start);
    uint16_t pc;
    uint8_t x, y;
    uint8_t sp;
	uint8_t sr;
    uint8_t ac; 
	uint8_t emu_flags;
	uint8_t interrupt_waiting;
    uint8_t mem[MEMORY_SIZE];
    // stores the address of memory modified by the last instruction
    uint16_t dirty_mem_addr;
    // the opcode of the last instruction run. for debugging only.
    uint8_t last_opcode;
};