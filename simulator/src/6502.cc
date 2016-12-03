/*
 




*/
#include "6502.h"
#include <stdint.h>
#include <cstring>
#include "Error.h"
#include <iomanip>
#include <bitset>
Emulator::Emulator(uint16_t pc_start) {
        pc = pc_start;
	sr = FLAG_INTERRUPT | 0x20;
	sp = 0xFF;
	interrupt_waiting = 0x00;
	memset(mem, 0xFF, MEMORY_SIZE);
}

Emulator::Emulator():
          pc(0X100)
        , sr(FLAG_INTERRUPT | 0x20)
        , interrupt_waiting(0x00)
        , mem()

        {
            memset(mem, 0xFF, MEMORY_SIZE);
        }


uint16_t Emulator::ReadTwoBytes() {
    uint8_t lsb, msb;
    uint16_t addr;
    lsb = ReadMem(++pc);
    msb = ReadMem(++pc);
    addr = ((msb << 8 ) | lsb);
    return addr;
}

void Emulator::PrintStack() {
	for (uint8_t i = 0xFF; i > sp; i--){
		uint16_t stack_base = 0x0100;
		std::cout << (int) ReadMem(stack_base + i) << " ";
	}
	std::cout << std::endl;
	//std::cout << "AC X  Y  " << std::endl;
	//	std::cout << std::setfill('0') << std::setw(2) << std::hex << "A: " << std::setfill('0') << std::setw(2) <<  ac << " X: " << (int) x << " Y: " << (int) y << " SR: " << (int) sr << std::endl;
	//std::cout << "Status Register: SV BDIZC" << std::endl;
	//std::cout << "Status Register: " << std::bitset<8>(sr) << std::endl;	

}

bool Emulator::Decode(){
	PrintStack();
	uint8_t opcode = ReadMem(++pc);
	std::cout << std::hex << "Begin Decode... Opcode " << (int) ReadMem(pc) <<" @" << pc << "         ";
	std::cout  << "A: " <<(int) ac << " X: " << (int) x << " Y: " << (int) y << " SR: " << (int) sr << std::endl;

	switch(opcode) {
		case 0x00:
			return false;
			break;
		case 0x01:
			//std::cout << "Or Acc Indirect X" << std::endl;
			Ins_ora_ind_x(ReadMem(++pc)); // Tested
			break;
		case 0x06:
			Ins_asl_zp(ReadMem(++pc)); // Tested
			break;
		case 0x08:
			Ins_php(); // Tested
			break;
		case 0x09:
			Ins_ora_imm(ReadMem(++pc));
			break;
		case 0x0A:
			Ins_asl_acc(); // Tested
			break;
		case 0x0E:
			Ins_asl_abs(ReadTwoBytes()); // Tested
			break;
		case 0x10:
			//std::cout << "Branch Positive" << std::endl;
			Ins_bpl(ReadMem(++pc)); // Tested
			break;
		case 0x18:
			Ins_clc();
			break;
		case 0x1E:
			Ins_asl_abs_x(ReadTwoBytes()); // Tested
			break;
		case 0x20:
			//std::cout << "JSR" << std::endl;
			Ins_jsr(ReadTwoBytes()); // Tested
			break;
		case 0x24:
			Ins_bit_zp(ReadMem(++pc));
			break;
		case 0x28:
			Ins_plp();
			break;
		case 0x29:
			Ins_and_imm(ReadMem(++pc));
			break;
		case 0x30:
			Ins_bmi(ReadMem(++pc));
			break;
		case 0x38:
			Ins_sec();
			break;
		case 0x48:
			Ins_pha();
			break;
		case 0x49:
			Ins_eor_imm(ReadMem(++pc));
			break;
		case 0x4C:
			//std::cout << "JMP Abs" << std::endl;
			Ins_jmp_abs(ReadTwoBytes()); // Tested
			break;
		case 0x50:
			Ins_bvc(ReadMem(++pc));
			break;
		case 0x58:
			Ins_cli();
			break;
		case 0x60:
			//std::cout << "Return to Subroutine" << std::endl;
			Ins_rts(); // Tested?
			break;
		case 0x68:
			Ins_pla();
			break;
		case 0x70:
			Ins_bvs(ReadMem(++pc));
			break;
		case 0x78:
			Ins_sei();
			break;
		// case 0x80:
		// 	break;
		case 0x85:
			Ins_sta_zp(ReadMem(++pc));
			break;
		case 0x86:
			Ins_stx_zp(ReadMem(++pc));
			break;
		case 0x88:
			//std::cout << "Decrement Y" << std::endl;
			Ins_dey();
			break;
		case 0x8C:
			Ins_sty_abs(ReadTwoBytes());
			break;
		case 0x8D:
			Ins_sta_abs(ReadTwoBytes());
			break;
		case 0x8E:
			Ins_stx_abs(ReadTwoBytes());
			break;
		case 0x90:
			Ins_bcc(ReadMem(++pc));
			break;
		case 0x91:
			Ins_sta_ind_y(ReadMem(++pc));
			break;
		case 0x9A:
			Ins_txs_x_sp(); // Tested
			break;
		case 0x9D:
			Ins_sta_abs_x(ReadTwoBytes());
			break;
		case 0xA0:
			Ins_ldy_imm(ReadMem(++pc)); // Tested
			break;
		case 0xA2:
			Ins_ldx_imm(ReadMem(++pc)); // Tested
			break;
		case 0xA5:
			Ins_lda_zer(ReadMem(++pc)); // Tested
			break;
		case 0xA6:
			Ins_ldx_zer(ReadMem(++pc)); // Tested
			break;
		case 0xA8:
			Ins_tay(); // Tested
			break;
		case 0xA9:
			Ins_lda_imm(ReadMem(++pc)); // Tested
			break;
		case 0xAC:
			//std::cout << "Ldy abs " << (int) ReadMem((pc+2)) << (int) ReadMem((pc+1)) << std::endl;
			Ins_ldy_abs(ReadTwoBytes()); // Tested
			break;
		case 0xB0:
			Ins_bcs(ReadMem(++pc));
			break;
		case 0xB5:
			Ins_lda_zerx(ReadMem(++pc)); // Tested
			break;
		case 0xB8:
			Ins_clv();
			break;
		case 0xBA:
			Ins_tsx();
			break;
		case 0xBD:
			Ins_lda_absx(ReadTwoBytes()); // Tested
			break;
		case 0xC8:
			std::cout << "Inc Y" << std::endl;
			Ins_inc_y(); // Tested
			break;
		case 0xC9:
			Ins_cmp_imm(ReadMem(++pc));
			break;
		case 0xC0:
			//std::cout << "Compare Y to immediate" << std::endl;
			Ins_cpy_imm(ReadMem(++pc)); // Tested
			break;
		case 0xCA:
			Ins_dex(); // Tested
			break;
		case 0xD0:
			//std::cout << "Branch If not Equal (!Zero Flag)" << std::endl;
			Ins_bne(ReadMem(++pc)); // Tested
			break;
		case 0xD8:
			Ins_cld(); // Tested
			//std::cout << "Clear Decimal Flag" << std::endl;
			break;
		case 0xEA:
			//NOP
			//std::cout << "NOP" << std::endl;
			break;
		case 0xF8:
			Ins_sed();
			break;
		case 0xF0:
			//std::cout << "Branh if Equal (zero Flag)" << std::endl;
			Ins_beq(ReadMem(++pc)); // Tested
			break;
		default:
			std::cout << "Error: Opcode '" << std::setfill('0')<< std::setw(2) << (int) opcode << "' Not Implemented" << std::endl;
			return false;
	}
	// std::cout << std::hex << pc << std::endl;
	//pc++;
	return true;
}

void Emulator::SetFlag(bool set, uint8_t Flag) {
	// std::cout << "Status Register: SV BDIZC" << std::endl;
	// std::cout << "Status Register: " << std::bitset<8>(sr) << std::endl;
	// std::cout << "Setting Flag: " << std::bitset<8>(Flag) << " To Value: " << set << std::endl;
	if (set) {
		sr |= Flag;
	} else {
		sr &= (0xFF - Flag);
	}
	// std::cout << "Status Register: SV BDIZC" << std::endl;
        // std::cout << "Status Register: " << std::bitset<8>(sr) << std::endl;
}

void Emulator::SetBaseAddr(const uint16_t& pc_start)
{
    SetPC(pc_start);
}

void Emulator::SetPC(const uint16_t& pc_pos)
{
    pc=pc_pos;
}

uint16_t Emulator::getPC()
{
    return pc;
}

bool Emulator::TestFlag(uint8_t Flag) {
	return (sr & Flag);
}

void Emulator::WriteMem(uint16_t address, uint8_t value) {
    mem[address] = value;
}

uint8_t Emulator::ReadMem(uint16_t address) {
	//std::cout << "Reading mem at address: " << address << std::endl;
	return(mem[address]);
}


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
}

uint8_t Emulator::StackPop() {
	if(sp == 0xFF) {
		sp = 0x00;
	} else {
		sp ++;
	}
	return Emulator::ReadMem(0x100 + sp);
}

void Emulator::Ins_jsr(uint16_t destination) {
	pc--;
    Emulator::StackPush((pc >> 8) & 0xFF);
    Emulator::StackPush(pc & 0xFF);
	pc = destination-1;
}

void Emulator::Ins_rts() {
	uint8_t lsb, msb;
	lsb = StackPop();
	msb = StackPop();

    // AAAAAAAAA00000000 or BBBBBBBB = AAAAAAAABBBBBBBB
    // plus 1 because return to the next instructionc
	pc = ((msb << 8 ) | lsb)+1;
}

void Emulator::Ins_jmp_abs(uint16_t destination) {
	pc = destination-1;
}

//void Emulator::Ins_ldx()

void Emulator::Ins_ldx_imm(uint8_t value) {
	x = value;
	SetFlag((x & 0x80),FLAG_NEGATIVE);
	SetFlag(!x,FLAG_ZERO);
}

void Emulator::Ins_ldy_imm(uint8_t value) {
	y = value;
	SetFlag((y & 0x80),FLAG_NEGATIVE);
	SetFlag(!y,FLAG_ZERO);
}

void Emulator::Ins_lda_imm(uint8_t value) {
	ac = value;
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
	SetFlag(!ac,FLAG_ZERO);
}

void Emulator::Ins_ldx_zer(uint8_t zero_addr) {
	x = Emulator::ReadMem(zero_addr);
	SetFlag((x & 0x80),FLAG_NEGATIVE);
	SetFlag(!x,FLAG_ZERO);
}

void Emulator::Ins_ldy_zer(uint8_t zero_addr) {
	y = Emulator::ReadMem(zero_addr);
}

void Emulator::Ins_lda_zer(uint8_t zero_addr) {
	ac = Emulator::ReadMem(zero_addr);
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
	SetFlag(!ac,FLAG_ZERO);
}

void Emulator::Ins_ldx_zery(uint8_t zero_addr) {
	uint16_t addr = ((zero_addr + y)%0x100);
	x = Emulator::ReadMem(addr);
	SetFlag((x & 0x80),FLAG_NEGATIVE);
	SetFlag(!x,FLAG_ZERO);
}

void Emulator::Ins_ldy_zerx(uint8_t zero_addr) {
	uint16_t addr = ((zero_addr + x)%0x100);
	y = Emulator::ReadMem(addr);
}

void Emulator::Ins_lda_zerx(uint8_t zero_addr) {
	uint16_t addr = ((zero_addr + x)%0x100);
	ac = Emulator::ReadMem(addr);
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
	SetFlag(!ac,FLAG_ZERO);
}


void Emulator::Ins_ldx_abs(uint16_t addr) {
	x = Emulator::ReadMem(addr);
	SetFlag((x & 0x80),FLAG_NEGATIVE);
	SetFlag(!x,FLAG_ZERO);
}

void Emulator::Ins_ldy_abs(uint16_t addr) {
	y = Emulator::ReadMem(addr);
}

void Emulator::Ins_lda_abs(uint16_t addr) {
	ac = Emulator::ReadMem(addr);
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
	SetFlag(!ac,FLAG_ZERO);
}

void Emulator::Ins_ldx_absy(uint16_t addr) {
	addr += y;
	x = Emulator::ReadMem(addr);
	SetFlag((x & 0x80),FLAG_NEGATIVE);
	SetFlag(!x,FLAG_ZERO);
}

void Emulator::Ins_ldy_absx(uint16_t addr) {
	addr += x;
	y = Emulator::ReadMem(addr);
}

void Emulator::Ins_lda_absx(uint16_t addr) {
	addr += x;
	ac = Emulator::ReadMem(addr);
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
	SetFlag(!ac,FLAG_ZERO);
}

void Emulator::Ins_lda_absy(uint16_t addr) {
	addr += y;
	ac = Emulator::ReadMem(addr);
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
	SetFlag(!ac,FLAG_ZERO);
}

void Emulator::Ins_adc_imm(uint8_t value)  //69", "IME", "SKIP", "SKIP")
{
	
 //    unsigned int temp = src + AC + (IF_CARRY() ? 1 : 0);
 //    SET_ZERO(temp & 0xff);	/* This is not valid in decimal mode */
 //    if (IF_DECIMAL()) {
 //        if (((AC & 0xf) + (src & 0xf) + (IF_CARRY() ? 1 : 0)) > 9) temp += 6;
	// 		SET_SIGN(temp);
	// SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));
	// if (temp > 0x99) temp += 96;
	// 	SET_CARRY(temp > 0x99);
 //    } else {
	// 	SET_SIGN(temp);
	// 	SET_OVERFLOW(!((AC ^ src) & 0x80) && ((AC ^ temp) & 0x80));
	// 	SET_CARRY(temp > 0xff);
 //    }
 //    AC = ((BYTE) temp);


	// ac &= value;
	// SetFlag((ac & 0x80),FLAG_NEGATIVE);
	// SetFlag(!ac,FLAG_ZERO);
}


void Emulator::ExecuteInst_adc_zp()  //65", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_adc_zp_x()  //75", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_adc_abs()  //6D", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_adc_abs_x()  //7D", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_adc_abs_y()  //79", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_adc_ind_x()  //61", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_adc_ind_y()  //71", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_and_imm(uint8_t value)  //29", "IME", "SKIP", "SKIP")
{
	ac &= value;
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
	SetFlag(!ac,FLAG_ZERO);
}


void Emulator::ExecuteInst_and_zp()  //25", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_and_zp_x()  //35", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_and_abs()  //2D", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_and_abs_x()  //3D", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_and_abs_y()  //39", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_and_ind_x()  //21", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_and_ind_y()  //31", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_asl_acc()  //0A", "SKIP", "SKIP", "SKIP")
{
	// std::cout << "Pre  Shift Accumulator = " << std::bitset<8>(ac) << std::endl;
	SetFlag((ac & 0x80),FLAG_CARRY);
	ac <<= 1;
	ac &= 0xFF;
	// std::cout << "Post Shift Accumulator = " << std::bitset<8>(ac) << std::endl;
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
	SetFlag(!ac,FLAG_ZERO);
	// std::cout << "Status Register: SV BDIZC" << std::endl;
	// std::cout << "Status Register: " << std::bitset<8>(sr) << std::endl;

}


void Emulator::Ins_asl_zp(uint8_t zero_addr)  //06", "SKIP", "REG", "SKIP")
{
	uint16_t value = ReadMem(zero_addr);
	SetFlag((value & 0x80),FLAG_CARRY);
	value <<= 1;
	value &= 0xFF;
	// std::cout << "Post Shift Accumulator = " << std::bitset<8>(ac) << std::endl;
	SetFlag((value & 0x80),FLAG_NEGATIVE);
	SetFlag(!value,FLAG_ZERO);
	WriteMem(zero_addr, value);
}


void Emulator::ExecuteInst_asl_zp_x()  //16", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_asl_abs(uint16_t address)  //0E", "SKIP", "REG", "SKIP")
{
	uint8_t value = ReadMem(address);
	SetFlag((value & 0x80),FLAG_CARRY);
	value <<= 1;
	value &= 0xFF;
	SetFlag((value & 0x80),FLAG_NEGATIVE);
	SetFlag(!value,FLAG_ZERO);
	WriteMem(address, value);
}


void Emulator::Ins_asl_abs_x(uint16_t abs_addr)  //1E", "SKIP", "REG", "OFFS")
{
	uint16_t final_addr = (abs_addr + x);
	uint8_t value = ReadMem(final_addr);
	SetFlag((value & 0x80),FLAG_CARRY);
	value <<= 1;
	value &= 0xFF;
	SetFlag((value & 0x80),FLAG_NEGATIVE);
	SetFlag(!value,FLAG_ZERO);
	WriteMem(final_addr, value);
}


void Emulator::Ins_bit_zp(uint8_t zero_addr)  //24", "SKIP", "REG", "SKIP")
{
	uint8_t value = ReadMem(zero_addr);
	SetFlag((value & 0x80), FLAG_NEGATIVE);
	SetFlag((value & 0x40), FLAG_OVERFLOW);
    value &= ac;
    SetFlag((!value), FLAG_ZERO);
}


void Emulator::ExecuteInst_bit_abs()  //2C", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_bpl(uint8_t rel_address)  //10", "SKIP", "SKIP", "SKIP")
{
	if (!TestFlag(FLAG_NEGATIVE)) {
	    uint8_t lsb, msb;
	    uint16_t destination;
	    msb = ((pc >> 8) & 0xFF);
	    lsb = (pc & 0xFF);
	    lsb = ((lsb + rel_address)%0x100);
	    destination = ((msb << 8) | lsb);
		pc = destination;
	}
}


void Emulator::Ins_bmi(uint8_t rel_address)  //30", "SKIP", "SKIP", "SKIP")
{
	if (TestFlag(FLAG_NEGATIVE)) {
	    uint8_t lsb, msb;
	    uint16_t destination;
	    msb = ((pc >> 8) & 0xFF);
	    lsb = (pc & 0xFF);
	    lsb = ((lsb + rel_address)%0x100);
	    destination = ((msb << 8) | lsb);
		pc = destination;
	}
}


void Emulator::Ins_bvc(uint8_t rel_address)  //50", "SKIP", "SKIP", "SKIP")
{
	if (!TestFlag(FLAG_OVERFLOW)) {
		uint8_t lsb, msb;
	    uint16_t destination;
	    msb = ((pc >> 8) & 0xFF);
	    lsb = (pc & 0xFF);
	    lsb = ((lsb + rel_address)%0x100);
	    destination = ((msb << 8) | lsb);
		pc = destination;
	}
}


void Emulator::Ins_bvs(uint8_t rel_address)  //70", "SKIP", "SKIP", "SKIP")
{
	if (TestFlag(FLAG_OVERFLOW)) {
		uint8_t lsb, msb;
	    uint16_t destination;
	    msb = ((pc >> 8) & 0xFF);
	    lsb = (pc & 0xFF);
	    lsb = ((lsb + rel_address)%0x100);
	    destination = ((msb << 8) | lsb);
		pc = destination;
	}
}


void Emulator::Ins_bcc(uint8_t rel_address)  //90", "SKIP", "SKIP", "SKIP")
{
	if (!TestFlag(FLAG_CARRY)) {
		uint8_t lsb, msb;
	    uint16_t destination;
	    msb = ((pc >> 8) & 0xFF);
	    lsb = (pc & 0xFF);
	    lsb = ((lsb + rel_address)%0x100);
	    destination = ((msb << 8) | lsb);
		pc = destination;
	}
}


void Emulator::Ins_bcs(uint8_t rel_address)  //B0", "SKIP", "SKIP", "SKIP")
{
	if (TestFlag(FLAG_CARRY)) {
		uint8_t lsb, msb;
	    uint16_t destination;
	    msb = ((pc >> 8) & 0xFF);
	    lsb = (pc & 0xFF);
	    lsb = ((lsb + rel_address)%0x100);
	    destination = ((msb << 8) | lsb);
		pc = destination;
	}
}


void Emulator::Ins_bne(uint8_t rel_address)  //D0", "SKIP", "SKIP", "SKIP")
{
	if (!TestFlag(FLAG_ZERO)) {
		uint8_t lsb, msb;
	    uint16_t destination;
	    msb = ((pc >> 8) & 0xFF);
	    lsb = (pc & 0xFF);
	    lsb = ((lsb + rel_address)%0x100);
	    destination = ((msb << 8) | lsb);
		pc = destination;
	}
}

void Emulator::Ins_beq(uint8_t rel_address)  //F0", "SKIP", "SKIP", "SKIP")
{
	//std::cout << (int) TestFlag(FLAG_ZERO) << std::endl;
	if (TestFlag(FLAG_ZERO)) {
		uint8_t lsb, msb;
	    uint16_t destination;
	    msb = ((pc >> 8) & 0xFF);
	    lsb = (pc & 0xFF);
	    lsb = ((lsb + rel_address)%0x100);
	    destination = ((msb << 8) | lsb);
		pc = destination;
	}
}

void Emulator::ExecuteInst_brk()  //00", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_cmp_imm(uint8_t value)  //C9", "IME", "SKIP", "SKIP")
{
	uint16_t result = ac - value;
	SetFlag(((result) < 0x100) , FLAG_CARRY);
	SetFlag((result & 0x80) , FLAG_NEGATIVE);
	SetFlag((!result) , FLAG_ZERO);
}


void Emulator::Ins_cmp_zp(uint8_t zero_addr)  //C5", "SKIP", "REG", "SKIP")
{
	uint8_t value = ReadMem(zero_addr);
	uint16_t result = ac - value;
	SetFlag(((ac + value) > 0x100) , FLAG_CARRY);
	SetFlag((result & 0x80) , FLAG_NEGATIVE);
	SetFlag((!result) , FLAG_ZERO);
}


void Emulator::ExecuteInst_cmp_zp_x()  //D5", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_cmp_abs(uint16_t address)  //CD", "SKIP", "REG", "SKIP")
{
	uint8_t value = ReadMem(address);
	uint16_t result = ac - value;
	SetFlag(((ac + value) > 0x100) , FLAG_CARRY);
	SetFlag((result & 0x80) , FLAG_NEGATIVE);
	SetFlag((!result) , FLAG_ZERO);
}


void Emulator::ExecuteInst_cmp_abs_x()  //DD", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_cmp_abs_y()  //D9", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_cmp_ind_x()  //C1", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_cmp_ind_y()  //D1", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_cpx_imm()  //E0", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_cpx_zp()  //E4", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_cpx_abs()  //EC", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_cpy_imm(uint8_t value)  //C0", "IME", "SKIP", "SKIP")
{
	std::cout << "Value " << (int) value << " Y "<< (int) y << std::endl;
	uint16_t result = y - value;
	SetFlag(((y + value) > 0x100) , FLAG_CARRY);
	SetFlag((result & 0x80) , FLAG_NEGATIVE);
	SetFlag((result == 0) , FLAG_ZERO);
}


void Emulator::ExecuteInst_cpy_zp()  //C4", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_cpy_abs()  //CC", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_dec_zp()  //C6", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_dec_zp_x()  //D6", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_dec_abs()  //CE", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_dec_abs_x()  //DE", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_eor_imm(uint8_t value)  //49", "IME", "SKIP", "SKIP")
{
	ac ^= value;
	SetFlag((ac & 0x80) , FLAG_NEGATIVE);
	SetFlag((ac == 0) , FLAG_ZERO);
}


void Emulator::ExecuteInst_eor_zp()  //45", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_eor_zp_x()  //55", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_eor_abs()  //4D", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_eor_abs_x()  //5D", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_eor_abs_y()  //59", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_eor_ind_x()  //41", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_eor_ind_y()  //51", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_clc()  //18", "SKIP", "SKIP", "SKIP")
{
	SetFlag(0, FLAG_CARRY);
}


void Emulator::Ins_sec()  //38", "SKIP", "SKIP", "SKIP")
{
	SetFlag(1, FLAG_CARRY);
}


void Emulator::Ins_cli()  //58", "SKIP", "SKIP", "SKIP")
{
	SetFlag(0, FLAG_INTERRUPT);
}


void Emulator::Ins_sei()  //78", "SKIP", "SKIP", "SKIP")
{
	SetFlag(1, FLAG_INTERRUPT);
}


void Emulator::Ins_clv()  //B8", "SKIP", "SKIP", "SKIP")
{
	SetFlag(0, FLAG_OVERFLOW);
}


void Emulator::Ins_cld()  //D8", "SKIP", "SKIP", "SKIP")
{
	SetFlag(0, FLAG_DECIMAL);
}


void Emulator::Ins_sed()  //F8", "SKIP", "SKIP", "SKIP")
{
	SetFlag(1, FLAG_DECIMAL);
}


void Emulator::ExecuteInst_inc_zp()  //E6", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_inc_zp_x()  //F6", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_inc_abs()  //EE", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_inc_abs_x()  //FE", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_jmp_abs()  //4C", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_jmp_ind()  //6C", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_jsr_abs()  //20", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}




void Emulator::Ins_ora_imm(uint8_t value)  //09", "IME", "SKIP", "SKIP")
{
	ac |= value;
	SetFlag((ac & 0x80), FLAG_NEGATIVE);
    SetFlag(!ac, FLAG_ZERO);
}


void Emulator::ExecuteInst_ora_zp()  //05", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ora_zp_x()  //15", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ora_abs()  //0D", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ora_abs_x()  //1D", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ora_abs_y()  //19", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_ora_ind_x(uint8_t addr)  //01", "SKIP", "REG", "OFFS")
{
	uint16_t address;
	uint8_t value;
	addr = (addr + x);
	uint8_t lsb = ReadMem(addr++);
	uint8_t msb = ReadMem(addr);
	address = ((msb << 8 ) | lsb);
	value = ReadMem(address);
	ac |= value;
	SetFlag((ac & 0x80), FLAG_NEGATIVE);
    SetFlag(!ac, FLAG_ZERO);
}


void Emulator::ExecuteInst_ora_ind_y()  //11", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_tax_a_x()  //AA", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_txa_x_a()  //8A", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_dex()  //CA", "SKIP", "SKIP", "SKIP")
{
//	if (x == 0x00) {
//		x = 0xFF;
//		SetFlag(1, FLAG_CARRY);
	//} else {
		x--;
	//}
	SetFlag((x & 0x80),FLAG_NEGATIVE);
    SetFlag((!x),FLAG_ZERO);
}


void Emulator::ExecuteInst_inx_x()  //E8", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_tay()  //A8", "SKIP", "SKIP", "SKIP")
{
	y = ac;
	SetFlag((y & 0x80), FLAG_NEGATIVE);
    SetFlag(!y, FLAG_ZERO);
}


void Emulator::ExecuteInst_tya_y_a()  //98", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_dey()  //88", "SKIP", "SKIP", "SKIP")
{
	y--;
	SetFlag((y & 0x80),FLAG_NEGATIVE);
    SetFlag((!y),FLAG_ZERO);
}


void Emulator::Ins_inc_y()  //C8", "SKIP", "SKIP", "SKIP")
{
	y = (y + 1)%0x100;
	SetFlag((y & 0x80), FLAG_NEGATIVE);
    SetFlag((!y), FLAG_ZERO);
}


void Emulator::ExecuteInst_rol_acc()  //2A", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_rol_zp()  //26", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_rol_zp_x()  //36", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_rol_abs()  //2E", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_rol_abs_x()  //3E", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ror_acc()  //6A", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ror_zp()  //66", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ror_zp_x()  //76", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ror_abs()  //6E", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ror_abs_x()  //7E", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_rti()  //40", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_rts()  //60", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sbc_imm()  //E9", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sbc_zp()  //E5", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sbc_zp_x()  //F5", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sbc_abs()  //ED", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sbc_abs_x()  //FD", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sbc_abs_y()  //F9", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sbc_ind_x()  //E1", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sbc_ind_y()  //F1", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_sta_zp(uint8_t zero_addr)  //85", "SKIP", "REG", "SKIP")
{
	Emulator::WriteMem(zero_addr, ac);
}


void Emulator::ExecuteInst_sta_zp_x()  //95", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_sta_abs(uint16_t address)  //8D", "SKIP", "REG", "SKIP")
{
	WriteMem(address, ac);
}


void Emulator::Ins_sta_abs_x(uint16_t address)  //9D", "SKIP", "REG", "OFFS")
{
	uint16_t addr = (address + x);
	Emulator::WriteMem(addr, ac);
}


void Emulator::ExecuteInst_sta_abs_y()  //99", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sta_ind_x()  //81", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_sta_ind_y(uint8_t start_address)  //91", "SKIP", "REG", "OFFS")
{
	uint8_t lsb,msb;
	uint16_t address;
	lsb = ReadMem(start_address++);
	msb = ReadMem(start_address);

    address = ((msb << 8 ) | lsb);
    address += y;
	Emulator::WriteMem(address, ac);
}


void Emulator::Ins_txs_x_sp()  //9A", "SKIP", "SKIP", "SKIP")
{
	StackPush(x);
}


void Emulator::Ins_tsx()  //BA", "SKIP", "SKIP", "SKIP")
{
	uint8_t x = StackPop();
	SetFlag((x & 0x80),FLAG_NEGATIVE);
    SetFlag((!x),FLAG_ZERO);
}


void Emulator::Ins_pha()  //48", "SKIP", "SKIP", "SKIP")
{
	StackPush(ac);
}


void Emulator::Ins_pla()  //68", "SKIP", "SKIP", "SKIP")
{
	ac = StackPop();
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
    SetFlag((!ac),FLAG_ZERO);	
}


void Emulator::Ins_php()  //08", "SKIP", "SKIP", "SKIP")
{
	SetFlag(1,FLAG_BREAK);
	StackPush(sr);
	SetFlag(0,FLAG_BREAK);
}


void Emulator::Ins_plp()  //28", "SKIP", "SKIP", "SKIP")
{
	sr = StackPop();
	SetFlag(0,FLAG_BREAK);
	SetFlag(1,0x20);
}


void Emulator::Ins_stx_zp(uint8_t address)  //86", "SKIP", "REG", "SKIP")
{
	WriteMem(address, x);
}


void Emulator::ExecuteInst_stx_zp_x()  //96", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_stx_abs(uint16_t address)  //8e", "SKIP", "REG", "SKIP")
{
	WriteMem(address, x);
}


void Emulator::ExecuteInst_sty_zp()  //84", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sty_zp_x()  //94", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_sty_abs(uint16_t address)  //8C", "SKIP", "REG", "SKIP")
{
	WriteMem(address, y);
}

// void Emulator::ExecuteInst_lda_imm()  //A9", "IME", "SKIP", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_lda_zp()  //A5", "SKIP", "REG", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_lda_zp_x()  //B5", "SKIP", "REG", "OFFS")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_lda_abs()  //AD", "SKIP", "REG", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_lda_abs_x()  //BD", "SKIP", "REG", "OFFS")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_lda_abs_y()  //B9", "SKIP", "REG", "OFFS")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_lda_ind_x()  //A1", "SKIP", "REG", "OFFS")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_lda_ind_y()  //B1", "SKIP", "REG", "OFFS")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_ldx_imm()  //A2", "IME", "SKIP", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_ldx_zp()  //A6", "SKIP", "REG", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_ldx_zp_x()  //B6", "SKIP", "REG", "OFFS")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_ldx_abs()  //AE", "SKIP", "REG", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_ldx_abs_x()  //BE", "SKIP", "REG", "OFFS")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_ldy_imm()  //A0", "IME", "SKIP", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_ldy_zp()  //A4", "SKIP", "REG", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_ldy_zp_x()  //B4", "SKIP", "REG", "OFFS")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_ldy_abs()  //AC", "SKIP", "REG", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_ldy_abs_x()  //BC", "SKIP", "REG", "OFFS")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_lsr_acc()  //4A", "SKIP", "SKIP", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_lsr_zp()  //46", "SKIP", "REG", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_lsr_zp_x()  //56", "SKIP", "REG", "OFFS")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_lsr_abs()  //4E", "SKIP", "REG", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


// void Emulator::ExecuteInst_lsr_abs_x()  //5E", "SKIP", "REG", "OFFS")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }

// C7E9 Check Ac 6f != 7f // so  the 0x20 flag?
// C800 Check Ac 64 != 74 // 0x68 opcode
// C817 Check Ac 2f != 35 // 0x68 opcode

