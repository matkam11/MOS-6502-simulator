/*
 




*/
#include "6502.h"
#include <stdint.h>
#include <cstring>
#include "Error.h"
#include <iomanip>

Emulator::Emulator(uint16_t pc_start) {
	pc = pc_start;
	sr = FLAG_INTERRUPT;;
	sp = 0xFF;
	interrupt_waiting = 0x00;
	memset(mem, 0xFF, MEMORY_SIZE);
};

bool Emulator::Decode(){
	std::cout << std::hex << pc << std::endl;
	uint8_t opcode = ReadMem(pc);
	switch(opcode) {
		case 0x01:
			Ins_ora_ind_x(ReadMem(pc++));
			break;
		case 0xA2:
			Ins_ldx_imm(ReadMem(pc++));
			break;
		default:
			std::cout << "Error: Opcode '" << std::setfill('0')<< std::setw(2) << (int) opcode << "' Not Implemented" << std::endl;
			return false;
	}
	std::cout << std::hex << pc << std::endl;
	pc++;
	return true;
}
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

//void Emulator::Ins_ldx()

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

void Emulator::Ins_ldx_zery(uint8_t zero_addr) {
	uint16_t addr = ((zero_addr + y)%0x100);
	x = Emulator::ReadMem(addr);
}

void Emulator::Ins_ldy_zerx(uint8_t zero_addr) {
	uint16_t addr = ((zero_addr + x)%0x100);
	y = Emulator::ReadMem(addr);
}

void Emulator::Ins_lda_zerx(uint8_t zero_addr) {
	uint16_t addr = ((zero_addr + x)%0x100);
	ac = Emulator::ReadMem(addr);
}


void Emulator::Ins_ldx_abs(uint16_t addr) {
	x = Emulator::ReadMem(addr);
}

void Emulator::Ins_ldy_abs(uint16_t addr) {
	y = Emulator::ReadMem(addr);
}

void Emulator::Ins_lda_abs(uint16_t addr) {
	ac = Emulator::ReadMem(addr);
}

void Emulator::Ins_ldx_absy(uint8_t msb) {
	uint16_t addr = ((msb << 8) | y);
	x = Emulator::ReadMem(addr);
}

void Emulator::Ins_ldy_absx(uint8_t msb) {
	uint16_t addr = ((msb << 8) | x);
	y = Emulator::ReadMem(addr);
}

void Emulator::Ins_lda_absx(uint8_t msb) {
	uint16_t addr = ((msb << 8) | x);
	ac = Emulator::ReadMem(addr);
}

void Emulator::Ins_lda_absy(uint8_t msb) {
	uint16_t addr = ((msb << 8) | y);
	ac = Emulator::ReadMem(addr);
}

void Emulator::ExecuteInst_adc_imm()  //69", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
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


void Emulator::ExecuteInst_and_imm()  //29", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
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


void Emulator::ExecuteInst_asl_acc()  //0A", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_asl_zp()  //06", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_asl_zp_x()  //16", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_asl_abs()  //0E", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_asl_abs_x()  //11", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_bit_zp()  //24", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_bit_abs()  //2C", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_br_on_pl()  //10", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_br_on_mi()  //30", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_br_on_vc()  //50", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_br_on_vs()  //70", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_br_on_cc()  //90", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_br_on_cs()  //B0", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_br_on_ne()  //D0", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_br_on_eq()  //F0", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_brk()  //00", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_cmp_imm()  //C9", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_cmp_zp()  //C5", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_cmp_zp_x()  //D5", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_cmp_abs()  //CD", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
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


void Emulator::ExecuteInst_cpy_imm()  //C0", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
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


void Emulator::ExecuteInst_eor_imm()  //49", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
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


void Emulator::ExecuteInst_flag_clc()  //18", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_flag_sec()  //38", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_flag_cli()  //58", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_flag_sei()  //78", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_flag_clv()  //B8", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_flag_cld()  //C8", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_flag_sed()  //D8", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
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


void Emulator::ExecuteInst_lda_imm()  //A9", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_lda_zp()  //A5", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_lda_zp_x()  //B5", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_lda_abs()  //AD", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_lda_abs_x()  //BD", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_lda_abs_y()  //B9", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_lda_ind_x()  //A1", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_lda_ind_y()  //B1", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ldx_imm()  //A2", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ldx_zp()  //A6", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ldx_zp_x()  //B6", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ldx_abs()  //AE", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ldx_abs_x()  //BE", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ldy_imm()  //A0", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ldy_zp()  //A4", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ldy_zp_x()  //B4", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ldy_abs()  //AC", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ldy_abs_x()  //BC", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_lsr_acc()  //4A", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_lsr_zp()  //46", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_lsr_zp_x()  //56", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_lsr_abs()  //4E", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_lsr_abs_x()  //5E", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_ora_imm()  //09", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
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


void Emulator::Ins_ora_ind_x(uint8_t msb)  //01", "SKIP", "REG", "OFFS")
{
	uint16_t addr = ((msb << 8) | y);
	uint8_t value = ReadMem(addr);
	ac |= value;
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


void Emulator::ExecuteInst_dex_x()  //CA", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_inx_x()  //E8", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_tay_a_y()  //A8", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_tya_y_a()  //98", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_dey_y()  //88", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_iny_y()  //C8", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
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


void Emulator::ExecuteInst_sta_zp()  //85", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sta_zp_x()  //95", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sta_abs()  //8D", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sta_abs_x()  //9D", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sta_abs_y()  //99", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sta_ind_x()  //81", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sta_ind_y()  //91", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_txs_x_sp()  //9A", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_tsx_sp_x()  //BA", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_pha_x_sp()  //48", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_pla_x_sp()  //68", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_php_x_sp()  //08", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_plp_x_sp()  //28", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_stx_zp()  //86", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_stx_zp_x()  //96", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_stx_abs()  //8e", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sty_zp()  //84", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sty_zp_x()  //94", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::ExecuteInst_sty_abs()  //8C", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}

