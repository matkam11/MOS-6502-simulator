/*
 




*/
#include "6502.h"

#include "Error.h"
#include "CommandLine.h"

#include <fstream>
#include <stdint.h>
#include <cstring>
#include <iomanip>
#include <bitset>



// Configuration options
uint16_t Emulator::base_addr = 0xBFF0;
uint16_t Emulator::start_addr = 0xBFFF;
bool Emulator::help = false;
const std::string Emulator::help_message =
        "The begging of the execution of the emulator depends on its start\n"
        "address and the address where the memory is loaded. "
        "The base address defaults to: 0xBFF0, and can be changed\n"
        "with propper configuration options.\n"
        "The base address defaults to: 0xBFFF, and can be changed\n"
        "with propper configuration options.\n"
        "\n";


void Emulator::RegisterOptions()
{
        // Get command line object
        misc::CommandLine *command_line = misc::CommandLine::getInstance();

        // Category
        command_line->setCategory("Emulator System");

        // Option sanity check
        command_line->RegisterUInt16("--emu-base-address <address>",
                        base_addr,
                        "This option sets the base address that is"
                        "going to be First decoded.");

        // Option sanity check
        command_line->RegisterUInt16("--emu-start-address <address>",
                        start_addr,
                        "This option sets the base address that is"
                        "going to be First decoded.");

        // Help message for memory system
        command_line->RegisterBool("--emu-help",
                        help,
                        "Print help message with special considaration"
                        "with regards the base address.");

}

void Emulator::ProcessOptions()
{
        SetStartAddr(start_addr);

        // Show help message
        if (help)
        {
                std::cerr << help_message;
                exit(0);
        }

        // Debug file
        // debug.setPath(debug_file);
}

Emulator::Emulator(uint16_t pc_start) {
    pc = pc_start;
	sr = FLAG_INTERRUPT | 0x20;
	sp = 0xFD;
	interrupt_waiting = 0x00;
	memset(mem, 0xFF, MEMORY_SIZE);
}

Emulator::Emulator():
          pc(start_addr)
        , sp(0xFD)
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

void Emulator::PrintMem() {

    std::cout<<std::endl
            <<"======== Mem contents from Base Address =========" <<std::endl;
    std::cout<<"Last from last 16 bytes"<<std::endl<<"\t - ";

    for (uint16_t i = base_addr; i < base_addr+256; i++)
    {
        if(i%16==0)
        {
            std::cout << std::setfill('0') << std::setw(2)
                      << std::hex<< (int) ReadMem(i) << std::endl;
            std::cout <<"0x" << std::setfill('0') << std::setw(2) << std::hex
                     << (int)i<<"\t - ";
        }
        else
        {
                std::cout << std::setfill('0') << std::setw(2)
                          << std::hex<< (int)ReadMem(i) << " ";
        }
    }
    std::cout <<". . ."<< std::endl;
    std::cout<<"===============================" <<std::endl<<std::endl;
}


uint8_t inline & Emulator::Address_acc_ptr() {
        return ac;
}

uint8_t inline & Emulator::Address_x_ptr() {
        return x;
}

uint8_t inline & Emulator::Address_y_ptr() {
        return y;
}

uint8_t inline & Emulator::Address_zp_ptr(uint8_t zero_addr) {
        return mem[zero_addr];
}

uint8_t inline & Emulator::Address_zp_x_ptr(uint8_t zero_addr) {
        return mem[zero_addr+x];
}


uint8_t inline & Emulator::Address_abs_ptr(uint16_t address) {
        return mem[address];
}

uint8_t inline &Emulator::Address_abs_x_ptr(uint16_t address) {
        return mem[(address+x)];
}

uint8_t inline &Emulator::Address_abs_y_ptr(uint16_t address) {
        return mem[(address+y)];
}

uint8_t inline Emulator::Address_zp(uint8_t zero_addr) {
	return Emulator::ReadMem(zero_addr);
}

uint8_t inline Emulator::Address_zp_x(uint8_t zero_addr) {
	return Emulator::ReadMem((zero_addr+x));
}

uint8_t inline Emulator::Address_zp_y(uint8_t zero_addr) {
	return Emulator::ReadMem((zero_addr+y));
}

uint8_t inline Emulator::Address_abs(uint16_t address) {
	return Emulator::ReadMem((address));
}

uint8_t inline Emulator::Address_abs_x(uint16_t address) {
	return Emulator::ReadMem((address+x));
}

uint8_t inline Emulator::Address_abs_y(uint16_t address) {
	return Emulator::ReadMem((address+y));
}

uint8_t inline Emulator::Address_ind_x(uint8_t start_address) {
	uint8_t lsb,msb;
	uint16_t address;
    start_address += x;
	lsb = ReadMem(start_address++);
	msb = ReadMem(start_address);

    address = ((msb << 8 ) | lsb);
	return ReadMem(address);
}

uint8_t inline Emulator::Address_ind_y(uint8_t start_address) {
	uint8_t lsb,msb;
	uint16_t address;
	lsb = ReadMem(start_address++);
	msb = ReadMem(start_address);

    address = ((msb << 8 ) | lsb);
    address += y;
	return ReadMem(address);
}

bool Emulator::Decode(){
	//PrintStack();
	uint8_t opcode = ReadMem(++pc);
	std::cout << std::hex << "Begin Decode... Opcode " << (int) ReadMem(pc) <<" @" << pc << "         ";
	std::cout  << "A: " <<(int) ac << " X: " << (int) x << " Y: " << (int) y << " SR: " << (int) sr << " SP: " << (int) sp << std::endl;

	switch(opcode) {
		case 0x00:
			return false;
			break;
		case 0x01:
			//std::cout << "Or Acc Indirect X" << std::endl
			//Ins_ora_ind_x(ReadMem(++pc)); // Tested
			Ins_ora_imm(Address_ind_x(ReadMem(++pc)));
			break;
		case 0x05:
			// ORA zp addressing mode
			Ins_ora_imm(Address_zp(ReadMem(++pc)));
			break;
		case 0x06:
			// Ins_asl_zp(ReadMem(++pc)); // Tested
			Ins_asl(Address_zp_ptr(ReadMem(++pc)));
			break;
		case 0x08:
			Ins_php(); // Tested
			break;
		case 0x09:
			Ins_ora_imm(ReadMem(++pc));
			break;
		case 0x0A:
			//Ins_asl_acc(); // Tested
			Ins_asl(Address_acc_ptr());
			break;
		case 0x0D:
			Ins_ora_imm(Address_abs(ReadTwoBytes()));
			break;
		case 0x0E:
			//Ins_asl_abs(ReadTwoBytes()); // Tested
			Ins_asl(Address_abs_ptr(ReadTwoBytes()));
			break;
		case 0x10:
			//std::cout << "Branch Positive" << std::endl;
			Ins_bpl(ReadMem(++pc)); // Tested
			break;
		case 0x11:
			Ins_ora_imm(Address_ind_y(ReadMem(++pc)));
			break;
		case 0x15:
			Ins_ora_imm(Address_zp_x(ReadMem(++pc)));
			break;
		case 0x18:
			Ins_clc();
			break;
		case 0x19:
			Ins_ora_imm(Address_abs_y(ReadTwoBytes()));
			break;
		case 0x1D:
			Ins_ora_imm(Address_abs_x(ReadTwoBytes()));
			break;
		case 0x1E:
			//Ins_asl_abs_x(ReadTwoBytes()); // Tested
			Ins_asl(Address_abs_x_ptr(ReadTwoBytes()));
			break;
		case 0x20:
			//std::cout << "JSR" << std::endl;
			Ins_jsr(ReadTwoBytes()); // Tested
			break;
		case 0x21:
			Ins_and_ind_x(ReadMem(++pc));
			break;
		case 0x24:
			Ins_bit_zp(ReadMem(++pc));
			break;
		case 0x25:
			Ins_and_imm(Address_zp(ReadMem(++pc)));
			break;
		case 0x26:
			Ins_rol(Address_zp_ptr(ReadMem(++pc)));
			break;
		case 0x28:
			Ins_plp();
			break;
		case 0x29:
			Ins_and_imm(ReadMem(++pc));
			break;
		case 0x2A:
			//Ins_rol_acc();
			Ins_rol(Address_acc_ptr());
			break;
		case 0x30:
			Ins_bmi(ReadMem(++pc));
			break;
		case 0x38:
			Ins_sec();
			break;
		case 0x40:
			Ins_rti();
			break;
		case 0x41:
			Ins_eor_imm(Address_ind_x(ReadMem(++pc)));
			break;
		case 0x45:
			Ins_eor_imm(Address_zp(ReadMem(++pc)));
			break;
		case 0x46:
			Ins_lsr_zp(ReadMem(++pc));
			break;
		case 0x48:
			Ins_pha();
			break;
		case 0x49:
			Ins_eor_imm(ReadMem(++pc));
			break;
		case 0x4A:
			Ins_lsr_acc();
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
		case 0x61:
                        Ins_adc(Address_ind_x(ReadMem(++pc)));
			break;
		case 0x65:
                        Ins_adc(Address_zp_ptr(ReadMem(++pc)));
			break;
		case 0x66:
			Ins_ror_zp(ReadMem(++pc));
			break;
		case 0x68:
			Ins_pla();
			break;
		case 0x69:
            Ins_adc(ReadMem(++pc));
			break;
		case 0x6A:
			Ins_ror_acc();
			break;
		case 0x70:
			Ins_bvs(ReadMem(++pc));
			break;
		case 0x77:
			Ins_extra_rra_zp_x(ReadMem(++pc));
			break;
		case 0x78:
			Ins_sei();
			break;
		case 0x81:
            Ins_sta(Address_ind_x(ReadMem(++pc)));
			break;
		case 0x84:
			Ins_sty_zp(ReadMem(++pc));
		 	break;
		case 0x85:
            Ins_sta(Address_zp_ptr(ReadMem(++pc)));
			break;
		case 0x86:
			Ins_stx_zp(ReadMem(++pc));
			break;
		case 0x88:
			//std::cout << "Decrement Y" << std::endl;
			Ins_dec(Address_y_ptr());
			break;
		case 0x8A:
			Ins_txa();
			break;
		case 0x8C:
			Ins_sty_abs(ReadTwoBytes());
			break;
		case 0x8D:
                        Ins_sta(ReadTwoBytes());
			break;
		case 0x8E:
			Ins_stx_abs(ReadTwoBytes());
			break;
		case 0x90:
			Ins_bcc(ReadMem(++pc));
			break;
		case 0x91:
                        Ins_sta(Address_ind_y(ReadMem(++pc)));
			break;
		case 0x98:
			Ins_tya();
			break;
		case 0x9A:
			Ins_txs_x_sp(); // Tested
			break;
		case 0x9D:
                        Ins_sta(Address_ind_y(ReadTwoBytes()));
			break;
		case 0xA0:
			Ins_ldy_imm(ReadMem(++pc)); // Tested
			break;
		case 0xA1:
			Ins_lda_imm(Address_ind_x(ReadMem(++pc)));
			break;
		case 0xA2:
			Ins_ldx_imm(ReadMem(++pc)); // Tested
			break;
		case 0xA4:
			Ins_ldy_imm(Address_zp(ReadMem(++pc)));
			break;
		case 0xA5:
			Ins_lda_imm(Address_zp(ReadMem(++pc))); // Tested
			break;
		case 0xA6:
			Ins_ldx_imm(Address_zp(ReadMem(++pc))); // Tested
			break;
		case 0xA8:
			Ins_tay(); // Tested
			break;
		case 0xA9:
			Ins_lda_imm(ReadMem(++pc)); // Tested
			break;
		case 0xAA:
			Ins_tax();
			break;
		case 0xAC:
			//std::cout << "Ldy abs " << (int) ReadMem((pc+2)) << (int) ReadMem((pc+1)) << std::endl;
			Ins_ldy_imm(Address_abs(ReadTwoBytes())); // Tested
			break;
		case 0xAD:
			Ins_lda_imm(Address_abs(ReadTwoBytes()));
			break;
		case 0xAE:
			Ins_ldx_imm(Address_abs(ReadTwoBytes()));
			break;
		case 0xB0:
			Ins_bcs(ReadMem(++pc));
			break;
		case 0xB1:
			Ins_lda_imm(Address_ind_y(ReadMem(++pc)));
			break;
		case 0xB5:
			Ins_lda_imm(Address_zp_x(ReadMem(++pc))); // Tested
			break;
		case 0xB8:
			Ins_clv();
			break;
		case 0xB9:
			Ins_lda_imm(Address_abs_y(ReadTwoBytes())); // Tested
			break;
		case 0xBA:
			Ins_tsx();
			break;
		case 0xBD:
			Ins_lda_imm(Address_abs_x(ReadTwoBytes())); // Tested
			break;
		case 0xC0:
			//std::cout << "Compare Y to immediate" << std::endl;
			Ins_cpy_imm(ReadMem(++pc)); // Tested
			break;
		case 0xC1:
            Ins_cmp_imm(Address_ind_y(ReadMem(++pc)));
			break;
		case 0xC4:
			Ins_cpy_imm(Address_zp(ReadMem(++pc)));
			break;
		case 0xC5:
			Ins_cmp_imm(Address_zp(ReadMem(++pc)));
			break;
		case 0xC6:
			Ins_dec(Address_zp_ptr(ReadMem(++pc)));
			break;
		case 0xC8:
			std::cout << "Inc Y" << std::endl;
			//Ins_inc_y(); // Tested
			Ins_inc(Address_y_ptr());
			break;
		case 0xC9:
			Ins_cmp_imm(ReadMem(++pc));
			break;
		case 0xCA:
			Ins_dec(Address_x_ptr()); // Tested
			break;
		case 0xCE:
			Ins_dec(Address_abs_ptr(ReadTwoBytes()));
			break;
		case 0xD0:
			//std::cout << "Branch If not Equal (!Zero Flag)" << std::endl;
			Ins_bne(ReadMem(++pc)); // Tested
			break;
		case 0xD6:
			Ins_dec(Address_zp_x_ptr(ReadMem(++pc)));
			break;
		case 0xD8:
			Ins_cld(); // Tested
			//std::cout << "Clear Decimal Flag" << std::endl;
			break;
		case 0xDE:
			Ins_dec(Address_abs_x_ptr(ReadTwoBytes()));
			break;
		case 0xE0:
			Ins_cpx_imm(ReadMem(++pc));
			break;
		case 0xE1:
			Ins_sbc_ind_x(ReadMem(++pc));
			break;
		case 0xE4:
			Ins_cpx_imm(Address_zp(ReadMem(++pc)));
			break;
		case 0xE5:
			Ins_sbc_imm(Address_zp(ReadMem(++pc)));
			break;
        case 0xE6:
        	std::cout << (int) ReadMem((pc+1)) << std::endl;
			Ins_inc(Address_zp_ptr(ReadMem(++pc)));
        	std::cout << (int) ReadMem((pc)) << std::endl;
			break;
		case 0xE8:
			Ins_inc(Address_x_ptr());
			break;
		case 0xE9:
			Ins_sbc_imm(ReadMem(++pc));
			break;
		case 0xEA:
			//NOP
			//std::cout << "NOP" << std::endl;
			break;
		case 0xEE:
			Ins_inc(Address_abs_ptr(ReadTwoBytes()));
			break;
		case 0xF0:
			//std::cout << "Branh if Equal (zero Flag)" << std::endl;
			Ins_beq(ReadMem(++pc)); // Tested
			break;
		case 0xF6:
			Ins_inc(Address_zp_x_ptr(ReadMem(++pc)));
			break;
		case 0xF8:
			Ins_sed();
			break;
		case 0xFE:
			Ins_inc(Address_abs_x_ptr(ReadTwoBytes()));
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

uint16_t Emulator::GetStartAddr()
{
    return start_addr;
}

uint16_t Emulator::GetBaseAddr()
{
    return base_addr;
}

void Emulator::SetStartAddr(const uint16_t& pc_start)
{
    SetPC(pc_start);
}

void Emulator::SetPC(const uint16_t& pc_pos)
{
    pc=pc_pos;
}

uint16_t Emulator::GetPC()
{
    return pc;
}

bool Emulator::TestFlag(uint8_t Flag) {
	return (sr & Flag);
}

void Emulator::WriteMem(uint16_t address, uint8_t value) {
    mem[address] = value;
}

uint8_t& Emulator::ReadMem(uint16_t address) {
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
	//pc--;
	std::cout << "PCH:" << (int) ((pc >> 8) & 0xFF) <<  " PCL:" << (int) (pc & 0xFF) << std::endl;

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
	pc = ((msb << 8 ) | lsb);
}

void Emulator::Ins_jmp_abs(uint16_t destination) {
	pc = destination-1;
}

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

void Emulator::Ins_adc(const uint8_t& value)  //69", "IME", "SKIP", "SKIP")
{
	uint temp = value + ac + TestFlag(FLAG_CARRY);
	SetFlag(!(temp & 0xFF),FLAG_ZERO);
	if (TestFlag(FLAG_DECIMAL)) {
		// if (((ac & 0xF) + (value & 0xF) + (TestFlag(FLAG_CARRY) & 0x1)) > 9) {
		// 	temp += 6;
		// }
		SetFlag((temp&0x80), FLAG_NEGATIVE);
		SetFlag(!((ac ^ value) & 0x80) && ((ac ^ temp) & 0x80), FLAG_OVERFLOW);
		// if (temp > 0x99) {
		// 	temp += 96;
		// }
		SetFlag((temp > 0x99), FLAG_CARRY);
	} else {
		SetFlag((temp&0x80), FLAG_NEGATIVE);
		SetFlag(!((ac ^ value) & 0x80) && ((ac ^ temp) & 0x80), FLAG_OVERFLOW);
		SetFlag((temp > 0xFF), FLAG_CARRY);
	}
	ac = temp & 0xFF;	
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


void Emulator::Ins_adc_ind_x(uint8_t start_address)  //61", "SKIP", "REG", "OFFS")
{
	uint8_t lsb,msb;
	uint16_t address;
    start_address += x;
	lsb = ReadMem(start_address++);
	msb = ReadMem(start_address);

    address = ((msb << 8 ) | lsb);
	uint8_t value = ReadMem(address);

	uint temp = value + ac + TestFlag(FLAG_CARRY);
	SetFlag(!(temp & 0xFF),FLAG_ZERO);
	if (TestFlag(FLAG_DECIMAL)) {
		// if (((ac & 0xF) + (value & 0xF) + (TestFlag(FLAG_CARRY) & 0x1)) > 9) {
		// 	temp += 6;
		// }
		SetFlag((temp&0x80), FLAG_NEGATIVE);
		SetFlag(!((ac ^ value) & 0x80) && ((ac ^ temp) & 0x80), FLAG_OVERFLOW);
		// if (temp > 0x99) {
		// 	temp += 96;
		// }
		SetFlag((temp > 0x99), FLAG_CARRY);
	} else {
		SetFlag((temp&0x80), FLAG_NEGATIVE);
		SetFlag(!((ac ^ value) & 0x80) && ((ac ^ temp) & 0x80), FLAG_OVERFLOW);
		SetFlag((temp > 0xFF), FLAG_CARRY);
	}
	ac = temp & 0xFF;	
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


void Emulator::Ins_and_ind_x(uint8_t start_address)  //21", "SKIP", "REG", "OFFS")
{
	uint8_t lsb,msb;
	uint16_t address;
    start_address += x;
	lsb = ReadMem(start_address++);
	msb = ReadMem(start_address);

    address = ((msb << 8 ) | lsb);
	uint8_t value = ReadMem(address);
	ac &= value;
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
	SetFlag(!ac,FLAG_ZERO);
}


void Emulator::ExecuteInst_and_ind_y()  //31", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}

void Emulator::Ins_asl(uint8_t &src)  //0A", "SKIP", "SKIP", "SKIP")
{
	// std::cout << "Pre  Shift Accumulator = " << std::bitset<8>(ac) << std::endl;
        SetFlag((src & 0x80),FLAG_CARRY);
        src <<= 1;
        src &= 0xFF;
	// std::cout << "Post Shift Accumulator = " << std::bitset<8>(ac) << std::endl;
        SetFlag((src & 0x80),FLAG_NEGATIVE);
        SetFlag(!src,FLAG_ZERO);
	// std::cout << "Status Register: SV BDIZC" << std::endl;
	// std::cout << "Status Register: " << std::bitset<8>(sr) << std::endl;

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
	    if (rel_address & 0x80) {
		    lsb -= (rel_address & 0x7F);
		    destination = ((msb << 8) | lsb);
	    } else {
		    destination = ((msb << 8) | lsb) + rel_address;
	    }
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
	    if (rel_address & 0x80) {
		    lsb -= (rel_address & 0x7F);
		    destination = ((msb << 8) | lsb);
	    } else {
		    destination = ((msb << 8) | lsb) + rel_address;
	    }
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
	    if (rel_address & 0x80) {
		    lsb -= (rel_address & 0x7F);
		    destination = ((msb << 8) | lsb);
	    } else {
		    destination = ((msb << 8) | lsb) + rel_address;
	    }
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
	    if (rel_address & 0x80) {
		    lsb -= (rel_address & 0x7F);
		    destination = ((msb << 8) | lsb);
	    } else {
		    destination = ((msb << 8) | lsb) + rel_address;
	    }
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
	    if (rel_address & 0x80) {
		    lsb -= (rel_address & 0x7F);
		    destination = ((msb << 8) | lsb);
	    } else {
		    destination = ((msb << 8) | lsb) + rel_address;
	    }
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
	    if (rel_address & 0x80) {
		    lsb -= (rel_address & 0x7F);
		    destination = ((msb << 8) | lsb);
	    } else {
		    destination = ((msb << 8) | lsb) + rel_address;
	    }
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
	    if (rel_address & 0x80) {
		    lsb -= (rel_address & 0x7F);
		    destination = ((msb << 8) | lsb);
	    } else {
		    destination = ((msb << 8) | lsb) + rel_address;
	    }
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
	    if (rel_address & 0x80) {
		    lsb -= (rel_address & 0x7F);
		    destination = ((msb << 8) | lsb);
	    } else {
		    destination = ((msb << 8) | lsb) + rel_address;
	    }
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

// void Emulator::Ins_cmp_ind_x(uint8_t start_address)  //C1", "SKIP", "REG", "OFFS")
// {
// 	uint8_t lsb,msb;
// 	uint16_t address;
//     start_address += x;
// 	lsb = ReadMem(start_address++);
// 	msb = ReadMem(start_address);

//     address = ((msb << 8 ) | lsb);
// 	uint8_t value = ReadMem(address);

// 	uint16_t result = ac - value;
// 	SetFlag(((result) < 0x100) , FLAG_CARRY);
// 	SetFlag((result & 0x80) , FLAG_NEGATIVE);
// 	SetFlag((!result) , FLAG_ZERO);
// }

void Emulator::Ins_cpx_imm(uint8_t value)  //E0", "IME", "SKIP", "SKIP")
{
	uint16_t result = x - value;
	SetFlag((result < 0x100) , FLAG_CARRY);
	SetFlag((result & 0x80) , FLAG_NEGATIVE);
	SetFlag((!result) , FLAG_ZERO);}


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
	uint16_t result = y - value;
	SetFlag((result < 0x100) , FLAG_CARRY);
	SetFlag((result & 0x80) , FLAG_NEGATIVE);
	SetFlag((!result) , FLAG_ZERO);
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


// void Emulator::ExecuteInst_eor_zp()  //45", "SKIP", "REG", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


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


void Emulator::Ins_inc(uint8_t &src)  //E6", "SKIP", "REG", "SKIP")
{
	uint8_t value = src;
	std::cout << (int) value << std::endl;
	value++;
	SetFlag((value & 0x80), FLAG_NEGATIVE);
    SetFlag((!value), FLAG_ZERO);
    src = value;
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


void Emulator::Ins_tax()  //AA", "SKIP", "SKIP", "SKIP")
{
	x = ac;
	SetFlag((x & 0x80),FLAG_NEGATIVE);
    SetFlag((!x),FLAG_ZERO);
}


void Emulator::Ins_txa()  //8A", "SKIP", "SKIP", "SKIP")
{
	ac = x;
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
    SetFlag((!ac),FLAG_ZERO);
}


void Emulator::Ins_dec(uint8_t &src)  //CA", "SKIP", "SKIP", "SKIP")
{
	src--;
	SetFlag((src & 0x80),FLAG_NEGATIVE);
    SetFlag((!src),FLAG_ZERO);
}

void Emulator::Ins_tay()  //A8", "SKIP", "SKIP", "SKIP")
{
	y = ac;
	SetFlag((y & 0x80), FLAG_NEGATIVE);
    SetFlag(!y, FLAG_ZERO);
}


void Emulator::Ins_tya()  //98", "SKIP", "SKIP", "SKIP")
{
	ac = y;
	SetFlag((ac & 0x80), FLAG_NEGATIVE);
    SetFlag(!ac, FLAG_ZERO);
}

void Emulator::Ins_rol(uint8_t &src)  //2A", "SKIP", "SKIP", "SKIP")
{
    uint16_t tempValue = (uint16_t) src;
    tempValue <<= 1;
    if (TestFlag(FLAG_CARRY)) {
    	tempValue |= 0x01;
    }
    SetFlag((tempValue > 0xFF), FLAG_CARRY);
    tempValue &= 0xFF;
    SetFlag((tempValue & 0x80), FLAG_NEGATIVE);
    SetFlag((!tempValue), FLAG_ZERO);
    src = tempValue;
}

void Emulator::Ins_rol_acc()  //2A", "SKIP", "SKIP", "SKIP")
{
	uint16_t tempValue = ac;
    tempValue <<= 1;
    if (TestFlag(FLAG_CARRY)) {
    	tempValue |= 0x01;
    }
    SetFlag((tempValue > 0xFF), FLAG_CARRY);
    tempValue &= 0xFF;
    SetFlag((tempValue & 0x80), FLAG_NEGATIVE);
    SetFlag((!tempValue), FLAG_ZERO);
    ac = tempValue;
}


void Emulator::Ins_rol_zp(uint8_t zero_address)  //26", "SKIP", "REG", "SKIP")
{
	uint16_t tempValue = ReadMem(zero_address);
    tempValue <<= 1;
    if (TestFlag(FLAG_CARRY)) {
    	tempValue |= 0x01;
    }
    SetFlag((tempValue > 0xFF), FLAG_CARRY);
    tempValue &= 0xFF;
    SetFlag((tempValue & 0x80), FLAG_NEGATIVE);
    SetFlag((!tempValue), FLAG_ZERO);
    WriteMem(zero_address,tempValue);
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


void Emulator::Ins_ror_acc()  //6A", "SKIP", "SKIP", "SKIP")
{
	uint16_t tempValue = ac;
    if (TestFlag(FLAG_CARRY)){
		tempValue |= 0x100;
	}
    SetFlag((tempValue & 0x01),FLAG_CARRY);
    tempValue >>= 1;
	SetFlag((tempValue & 0x80), FLAG_NEGATIVE);
    SetFlag(!tempValue, FLAG_ZERO);
    ac = tempValue;
}

void Emulator::Ins_ror_zp(uint8_t zero_addresss)  //66", "SKIP", "REG", "SKIP")
{
	uint16_t tempValue = ReadMem(zero_addresss);
    if (TestFlag(FLAG_CARRY)){
		tempValue |= 0x100;
	}
    SetFlag((tempValue & 0x01),FLAG_CARRY);
    tempValue >>= 1;
	SetFlag((tempValue & 0x80), FLAG_NEGATIVE);
    SetFlag(!tempValue, FLAG_ZERO);
    WriteMem(zero_addresss, tempValue);
}


void Emulator::ExecuteInst_ror_zp_x()  //76", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_ror_abs(uint16_t address)  //6E", "SKIP", "REG", "SKIP")
{
	uint8_t value = ReadMem(address);
    if (TestFlag(FLAG_CARRY)){
		value |= 0x100;
	}
    SetFlag((value & 0x01),FLAG_CARRY);
    value >>= 1;
	SetFlag((value & 0x80), FLAG_NEGATIVE);
    SetFlag(!value, FLAG_ZERO);
    WriteMem(address, value);
}


void Emulator::ExecuteInst_ror_abs_x()  //7E", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_rti()  //40", "SKIP", "SKIP", "SKIP")
{
	sr = StackPop();
	sr |= 0x20;
	pc = StackPop();
	pc |= (StackPop() << 8);
	pc--;
}


// void Emulator::ExecuteInst_rts()  //60", "SKIP", "SKIP", "SKIP")
// {
// 	throw misc::Panic("Unimplemented instruction");
// }


void Emulator::Ins_sbc_imm(uint8_t value)  //E9", "IME", "SKIP", "SKIP")
{
	uint16_t temp = ~value + ac + TestFlag(FLAG_CARRY);
	std::cout << "Temp Value: " <<  (int) temp << std::endl;
	SetFlag(!(temp & 0xFF),FLAG_ZERO);
	if (TestFlag(FLAG_DECIMAL)) {
		// if (((ac & 0xF) + (value & 0xF) + (TestFlag(FLAG_CARRY) & 0x1)) > 9) {
		// 	temp += 6;
		// }
		SetFlag((temp&0x80), FLAG_NEGATIVE);
		SetFlag(!((ac ^ value) & 0x80) && !((ac ^ temp) & 0x80), FLAG_OVERFLOW);
		// if (temp > 0x99) {
		// 	temp += 96;
		// }
		SetFlag(!(temp & 0x100), FLAG_CARRY);
	} else {
		SetFlag((temp&0x80), FLAG_NEGATIVE);
		SetFlag(!!((ac ^ value) & (ac ^ temp) & 0x80), FLAG_OVERFLOW);
		SetFlag(!(temp & 0x100), FLAG_CARRY);
	}
	ac = temp & 0xFF;	
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


void Emulator::Ins_sbc_ind_x(uint8_t start_address)  //E1", "SKIP", "REG", "OFFS")
{
	uint8_t lsb,msb;
	uint16_t address;
    start_address += x;
	lsb = ReadMem(start_address++);
	msb = ReadMem(start_address);

    address = ((msb << 8 ) | lsb);
	uint8_t value = ReadMem(address);

	uint16_t temp = ~value + ac + TestFlag(FLAG_CARRY);
	std::cout << "Temp Value: " <<  (int) temp << std::endl;
	SetFlag(!(temp & 0xFF),FLAG_ZERO);
	if (TestFlag(FLAG_DECIMAL)) {
		// if (((ac & 0xF) + (value & 0xF) + (TestFlag(FLAG_CARRY) & 0x1)) > 9) {
		// 	temp += 6;
		// }
		SetFlag((temp&0x80), FLAG_NEGATIVE);
		SetFlag(!((ac ^ value) & 0x80) && !((ac ^ temp) & 0x80), FLAG_OVERFLOW);
		// if (temp > 0x99) {
		// 	temp += 96;
		// }
		SetFlag(!(temp & 0x100), FLAG_CARRY);
	} else {
		SetFlag((temp&0x80), FLAG_NEGATIVE);
		SetFlag(!!((ac ^ value) & (ac ^ temp) & 0x80), FLAG_OVERFLOW);
		SetFlag(!(temp & 0x100), FLAG_CARRY);
	}
	ac = temp & 0xFF;	
}


void Emulator::ExecuteInst_sbc_ind_y()  //F1", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}

void Emulator::Ins_sta(const uint16_t& address)  //8D", "SKIP", "REG", "SKIP")
{
        WriteMem(address, ac);
}


void Emulator::Ins_txs_x_sp()  //9A", "SKIP", "SKIP", "SKIP")
{
	//StackPush(x);
	sp = x;
}


void Emulator::Ins_tsx()  //BA", "SKIP", "SKIP", "SKIP")
{
	//x = StackPop();
	x = sp;
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
	// ac |= 0x10;
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
    SetFlag((!ac),FLAG_ZERO);	
}


void Emulator::Ins_php()  //08", "SKIP", "SKIP", "SKIP")
{
	//SetFlag(1,FLAG_BREAK);
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


void Emulator::Ins_sty_zp(uint8_t address)  //84", "SKIP", "REG", "SKIP")
{
	WriteMem(address, y);
}


void Emulator::ExecuteInst_sty_zp_x()  //94", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Emulator::Ins_sty_abs(uint16_t address)  //8C", "SKIP", "REG", "SKIP")
{
	WriteMem(address, y);
}

void Emulator::Ins_extra_rra_zp_x(uint8_t zero_addr)
{
	uint16_t value = ReadMem((zero_addr+x));
	uint8_t carry = TestFlag(FLAG_CARRY) << 7;
	SetFlag((value & 0x01), FLAG_CARRY);
	value = (value >> 1) | carry;
	uint16_t result = ac + value + TestFlag(FLAG_CARRY);
	SetFlag(!!(~(ac ^ value) & (ac ^ result) & 0x80), FLAG_OVERFLOW);
	SetFlag((!!(result & 0x100)) , FLAG_CARRY);
	ac = result & 0xFF;
	SetFlag((!ac), FLAG_ZERO);
	SetFlag((ac&0x80), FLAG_NEGATIVE);
	//FZ = (A == 0);
	//FN = (A >> 7) & 1;
	WriteMem((zero_addr+x), value); //MemSet(CalcAddr, TmpData);


	// uint temp = value + ac + TestFlag(FLAG_CARRY);
	// SetFlag(!(temp & 0xFF),FLAG_ZERO);
	// if (TestFlag(FLAG_DECIMAL)) {
	// 	// if (((ac & 0xF) + (value & 0xF) + (TestFlag(FLAG_CARRY) & 0x1)) > 9) {
	// 	// 	temp += 6;
	// 	// }
	// 	SetFlag((temp&0x80), FLAG_NEGATIVE);
	// 	SetFlag(!((ac ^ value) & 0x80) && ((ac ^ temp) & 0x80), FLAG_OVERFLOW);
	// 	// if (temp > 0x99) {
	// 	// 	temp += 96;
	// 	// }
	// 	SetFlag((temp > 0x99), FLAG_CARRY);
	// } else {
	// 	SetFlag((temp&0x80), FLAG_NEGATIVE);
	// 	SetFlag(!((ac ^ value) & 0x80) && ((ac ^ temp) & 0x80), FLAG_OVERFLOW);
	// 	SetFlag((temp > 0xFF), FLAG_CARRY);
	// }
	// ac = temp & 0xFF;	
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


void Emulator::Ins_lsr_acc()  //4A", "SKIP", "SKIP", "SKIP")
{
	SetFlag((ac & 0x01), FLAG_CARRY);
	ac >>= 1;
	SetFlag(0, FLAG_NEGATIVE);
	SetFlag((!ac), FLAG_ZERO);
}


void Emulator::Ins_lsr_zp(uint8_t zero_addresss)  //46", "SKIP", "REG", "SKIP")
{
	uint8_t value = ReadMem(zero_addresss);
	SetFlag((value & 0x01), FLAG_CARRY);
	value >>= 1;
	SetFlag(0, FLAG_NEGATIVE);
	SetFlag((!value), FLAG_ZERO);
	WriteMem(zero_addresss, value);
}


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
// C817 Check Ac 2f != 3f // 0x68 opcode

// void Emulator::Ins_ldx_zp(uint8_t zero_addr) {
// 	x = Emulator::ReadMem(zero_addr);
// 	SetFlag((x & 0x80),FLAG_NEGATIVE);
// 	SetFlag(!x,FLAG_ZERO);
// }

// void Emulator::Ins_ldy_zp(uint8_t zero_addr) {
// 	y = Emulator::ReadMem(zero_addr);
// 	SetFlag((y & 0x80),FLAG_NEGATIVE);
// 	SetFlag(!y,FLAG_ZERO);
// }

// void Emulator::Ins_lda_zp(uint8_t zero_addr) {
// 	ac = Emulator::ReadMem(zero_addr);
// 	SetFlag((ac & 0x80),FLAG_NEGATIVE);
// 	SetFlag(!ac,FLAG_ZERO);
// }
