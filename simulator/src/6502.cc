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

void Emulator::Reset()
{
    pc = 0x0000;
    sr = FLAG_INTERRUPT | 0x20;
    sp = 0xFD;
    interrupt_waiting = 0x00;
    memset(mem, 0xFF, MEMORY_SIZE);

    x = 0x00;
    y = 0x00;
    ac= 0x00;
    emu_flags=FLAG_INTERRUPT | 0x20;

    // stores the address of memory modified by the last instruction
    dirty_mem_addr=0x00;
    // the opcode of the last instruction run. for debugging only.
    last_opcode=0x00;
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

void Emulator::MemoryWatch(uint16_t address) {
	std::cout << "Value @ " << (int)address << " = " << (int) ReadMem(address) << std::endl;
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

uint8_t inline & Emulator::Address_imm_ptr() {
		//std::cout << (int) imm << std::endl;
        return mem[++pc];
}

uint8_t inline & Emulator::Address_zp_ptr() {
    	uint8_t zero_addr = ReadMem(++pc);
        return mem[zero_addr];
}

uint8_t inline & Emulator::Address_zp_x_ptr() {
    	uint8_t zero_addr = ReadMem(++pc);
    	std::cout << "Value: " << (int) ReadMem(zero_addr+x) << " @ " << ((zero_addr+x)&0xff) << std::endl; 
        return mem[((zero_addr+x)&0xff)];
}

uint8_t inline & Emulator::Address_zp_y_ptr() {
    	uint8_t zero_addr = ReadMem(++pc);
        return mem[((zero_addr+y)&0xff)];
}

uint8_t inline & Emulator::Address_abs_ptr() {
		uint16_t address = ReadTwoBytes();
        return mem[address];
}

uint8_t inline &Emulator::Address_abs_x_ptr() {
		uint16_t address = ReadTwoBytes();
        return mem[(address+x)];
}

uint8_t inline &Emulator::Address_abs_y_ptr() {
		uint16_t address = ReadTwoBytes();
        return mem[(address+y)];
}

uint8_t inline &Emulator::Address_ind_x_ptr() {
	uint8_t start_address = ReadMem(++pc);
	uint8_t lsb,msb;
	uint16_t address;
    start_address += x;
	lsb = ReadMem(start_address++);
	msb = ReadMem(start_address);

    address = ((msb << 8 ) | lsb);
	return mem[address];
}

uint8_t inline & Emulator::Address_ind_y_ptr() {
	uint8_t start_address = ReadMem(++pc);
    std::cout << "Reading Start addr: " << (int) start_address << std::endl;
	uint8_t lsb,msb;
	uint16_t address;
	lsb = ReadMem(start_address++);
	msb = ReadMem(start_address);
    std::cout << "Reading addr: " << (int) msb << " " << (int) lsb << std::endl;
    address = ((msb << 8 ) | lsb);
    //address += y;
    std::cout << "Reading addr: " << (int) address << std::endl;
	return mem[address];
}

bool Emulator::Decode(){
	//PrintStack();
	uint8_t opcode = ReadMem(++pc);
	std::cout << std::hex << "Begin Decode... Opcode " << (int) ReadMem(pc) <<" @" << pc << "         ";
        std::cout << std::uppercase<< std::hex << "A:"
                  << std::setfill('0') << std::setw(2) <<(int) ac << " X:"
                  << std::setfill('0') << std::setw(2) << (int) x << " Y:"
                  << std::setfill('0') << std::setw(2) << (int) y << " P:"
                  << std::setfill('0') << std::setw(2) << (int) sr << " SP:"
                  << std::setfill('0') << std::setw(2) << (int) sp << std::endl;
    MemoryWatch(0x0000);
    MemoryWatch(0x0010);
    MemoryWatch(0x0011);
    MemoryWatch(0x0049);
    MemoryWatch(0x0080);
    MemoryWatch(0x0081); 
    MemoryWatch(0x0200);       
    MemoryWatch(0x0201);
    MemoryWatch(0x0548);
    MemoryWatch(0x0549);
    MemoryWatch(0x0647);
    MemoryWatch(0x0648);
	switch(opcode) {
		case 0x00:
			return false;
			break;
		case 0x01:
			//std::cout << "Or Acc Indirect X" << std::endl
			//Ins_ora_ind_x(ReadMem(++pc)); // Tested
			Ins_ora(Address_ind_x_ptr());
			break;
		case 0x03:
			Ins_extra_aso(Address_ind_x_ptr());
			break;
		case 0x04:
			Ins_extra_skb();
			break;
		case 0x05:
			Ins_ora(Address_zp_ptr());
			break;
		case 0x06:
			Ins_asl(Address_zp_ptr());
			break;
		case 0x07:
			Ins_extra_aso(Address_zp_ptr());
			break;
		case 0x08:
			Ins_php(); // Tested
			break;
		case 0x09:
			Ins_ora(Address_imm_ptr());
			break;
		case 0x0A:
			//Ins_asl_acc(); // Tested
			Ins_asl(Address_acc_ptr());
			break;
		case 0x0C:
			Ins_extra_skw();
			break;
		case 0x0D:
			Ins_ora(Address_abs_ptr());
			break;
		case 0x0E:
			Ins_asl(Address_abs_ptr());
			break;
		case 0x0F:
			Ins_extra_aso(Address_abs_ptr());
			break;
		case 0x10:
			Ins_bpl(ReadMem(++pc)); // Tested
			break;
		case 0x11:
			Ins_ora(Address_ind_y_ptr());
			break;
		case 0x13:
			Ins_extra_aso(Address_ind_y_ptr());
			break;
		case 0x14:
			Ins_extra_skb();
			break;
		case 0x15:
			Ins_ora(Address_zp_x_ptr());
			break;
		case 0x16:
			Ins_asl(Address_zp_x_ptr());
			break;
		case 0x17:
			Ins_extra_aso(Address_zp_x_ptr());
			break;
		case 0x18:
			Ins_clc();
			break;
		case 0x19:
			Ins_ora(Address_abs_y_ptr());
			break;
		case 0x1A:
			//#NOP
			break;
		case 0x1B:
			Ins_extra_aso(Address_abs_y_ptr());
			break;
		case 0x1C:
			Ins_extra_skw();
			break;
		case 0x1D:
			Ins_ora(Address_abs_x_ptr());
			break;
		case 0x1E:
			//Ins_asl_abs_x(ReadTwoBytes()); // Tested
			Ins_asl(Address_abs_x_ptr());
			break;
		case 0x1F:
			Ins_extra_aso(Address_abs_x_ptr());
			break;
		case 0x20:
			//std::cout << "JSR" << std::endl;
			Ins_jsr(ReadTwoBytes()); // Tested
			break;
		case 0x21:
			Ins_and(Address_ind_x_ptr());
			break;
		case 0x23:
			Ins_extra_rla(Address_ind_x_ptr());
			break;
		case 0x24:
			Ins_bit(Address_zp_ptr());
			break;
		case 0x25:
			Ins_and(Address_zp_ptr());
			break;
		case 0x26:
			Ins_rol(Address_zp_ptr());
			break;
		case 0x27:
			Ins_extra_rla(Address_zp_ptr());
			break;
		case 0x28:
			Ins_plp();
			break;
		case 0x29:
			Ins_and(Address_imm_ptr());
			break;
		case 0x2A:
			Ins_rol(Address_acc_ptr());
			break;
		case 0x2C:
			Ins_bit(Address_abs_ptr());
			break;	
		case 0x2D:
			Ins_and(Address_abs_ptr());
			break;
		case 0x2E:
			Ins_rol(Address_abs_ptr());
			break;
		case 0x2F:
			Ins_extra_rla(Address_abs_ptr());
			break;
		case 0x30:
			Ins_bmi(ReadMem(++pc));
			break;
		case 0x31:
			Ins_and(Address_ind_y_ptr());
			break;
		case 0x33:
			Ins_extra_rla(Address_ind_y_ptr());
			break;
		case 0x34:
			Ins_extra_skb();
			break;
		case 0x35:
			Ins_and(Address_zp_x_ptr());
			break;
		case 0x36:
			Ins_rol(Address_zp_x_ptr());
			break;
		case 0x37:
			Ins_extra_rla(Address_zp_x_ptr());
			break;
		case 0x38:
			Ins_sec();
			break;
		case 0x39:
			Ins_and(Address_abs_y_ptr());
			break;
		case 0x3A:
			//#NOP
			break;
		case 0x3B:
			Ins_extra_rla(Address_abs_y_ptr());
			break;
		case 0x3C:
			Ins_extra_skw();
			break;
		case 0x3D:
			Ins_and(Address_abs_x_ptr());
			break;
		case 0x3E: 
			Ins_rol(Address_abs_x_ptr());
			break;
		case 0x3F:
			Ins_extra_rla(Address_abs_x_ptr());
			break;
		case 0x40:
			Ins_rti();
			break;
		case 0x41:
			Ins_eor(Address_ind_x_ptr());
			break;
		case 0x43:
			Ins_extra_lse(Address_ind_x_ptr());
			break;
		case 0x44:
			Ins_extra_skb();
			break;
		case 0x45:
			Ins_eor(Address_zp_ptr());
			break;
		case 0x46:
			Ins_lsr(Address_zp_ptr());
			break;
		case 0x47:
			Ins_extra_lse(Address_zp_ptr());
			break;
		case 0x48:
			Ins_pha();
			break;
		case 0x49:
			Ins_eor(Address_imm_ptr());
			break;
		case 0x4A:
			Ins_lsr(Address_acc_ptr());
			break;
		case 0x4C:
			//std::cout << "JMP Abs" << std::endl;
			Ins_jmp_abs(ReadTwoBytes()); // Tested
			break;
		case 0x4D:
			Ins_eor(Address_abs_ptr());
			break;
		case 0x4E:
			Ins_lsr(Address_abs_ptr());
			break;
		case 0x4F:
			Ins_extra_lse(Address_abs_ptr());
			break;
		case 0x50:
			Ins_bvc(ReadMem(++pc));
			break;
		case 0x51:
			Ins_eor(Address_ind_y_ptr());
			break;
		case 0x53:
			Ins_extra_lse(Address_ind_y_ptr());
			break;
		case 0x54:
			Ins_extra_skb();
			break;
		case 0x55:
			Ins_eor(Address_zp_x_ptr());
			break;
		case 0x56:
			Ins_lsr(Address_zp_x_ptr());
			break;
		case 0x57:
			Ins_extra_lse(Address_zp_x_ptr());
			break;
		case 0x58:
			Ins_cli();
			break;
		case 0x59:
			Ins_eor(Address_abs_y_ptr());
			break;
		case 0x5A:
			//#NOP
			break;
		case 0x5B:
			Ins_extra_lse(Address_abs_y_ptr());
			break;
		case 0x5C:
			Ins_extra_skw();
			break;
		case 0x5D:
			Ins_eor(Address_abs_x_ptr());
			break;
		case 0x5E:
			Ins_lsr(Address_abs_x_ptr());
			break;
		case 0x5F:
			Ins_extra_lse(Address_abs_x_ptr());
			break;
		case 0x60:
			//std::cout << "Return to Subroutine" << std::endl;
			Ins_rts(); // Tested?
			break;
		case 0x61:
            Ins_adc(Address_ind_x_ptr());
			break;
		case 0x63:
			Ins_extra_rra(Address_ind_x_ptr());
			break;
		case 0x64:
			Ins_extra_skb();
			break;
		case 0x65:
            Ins_adc(Address_zp_ptr());
			break;
		case 0x66:
			Ins_ror(Address_zp_ptr());
			break;
		case 0x67:
			Ins_extra_rra(Address_zp_ptr());
			break;
		case 0x68:
			Ins_pla();
			break;
		case 0x69:
            Ins_adc(Address_imm_ptr());
			break;
		case 0x6A:
			Ins_ror(Address_acc_ptr());
			break;
		case 0x6C:
			Ins_jmp_ind(ReadTwoBytes());;
			break;
		case 0x6D:
			Ins_adc(Address_abs_ptr());
			break;
		case 0x6E:
			Ins_ror(Address_abs_ptr());
			break;
		case 0x6F:
			Ins_extra_rra(Address_abs_ptr());
			break;
		case 0x70:
			Ins_bvs(ReadMem(++pc));
			break;
		case 0x71:
			Ins_adc(Address_ind_y_ptr());
			break;
		case 0x73:
			Ins_extra_rra(Address_ind_y_ptr());
			break;
		case 0x74:
			Ins_extra_skb();
			break;
		case 0x75:
			Ins_adc(Address_zp_x_ptr());
			break;
		case 0x76:
			Ins_ror(Address_zp_x_ptr());
			break;
		case 0x77:
			Ins_extra_rra(Address_zp_x_ptr());
			break;
		case 0x78:
			Ins_sei();
			break;
		case 0x79:
			Ins_adc(Address_abs_y_ptr());
			break;
		case 0x7A:
			//#NOP
			break;
		case 0x7B:
			Ins_extra_rra(Address_abs_y_ptr());
			break;
		case 0x7C:
			Ins_extra_skw();
			break;
		case 0x7D:
			Ins_adc(Address_abs_x_ptr());
			break;
		case 0x7E:
			Ins_ror(Address_abs_x_ptr());
			break;
		case 0x7F:
			Ins_extra_rra(Address_abs_x_ptr());
			break;
		case 0x80:
			Ins_extra_skb();
			break;
		case 0x81:
            Ins_sta(Address_ind_x_ptr());
			break;
		case 0x82:
			Ins_extra_skb();
			break;
		case 0x83:
			Ins_extra_axs(Address_ind_x_ptr());
			break;
		case 0x84:
			Ins_sty(Address_zp_ptr());
		 	break;
		case 0x85:
            Ins_sta(Address_zp_ptr());
			break;
		case 0x86:
			Ins_stx(Address_zp_ptr());
			break;
		case 0x87:
			Ins_extra_axs(Address_zp_ptr());
			break;
		case 0x88:
			Ins_dec(Address_y_ptr());
			break;
		case 0x8A:
			Ins_txa();
			break;
		case 0x8C:
			Ins_sty(Address_abs_ptr());
			break;
		case 0x8D:
			Ins_sta(Address_abs_ptr());
			break;
		case 0x8E:
			Ins_stx(Address_abs_ptr());
			break;
		case 0x8F:
			Ins_extra_axs(Address_abs_ptr());
			break;
		case 0x90:
			Ins_bcc(ReadMem(++pc));
			break;
		case 0x91:
            Ins_sta(Address_ind_y_ptr());
			break;
		case 0x94:
			Ins_sty(Address_zp_x_ptr());
			break;
		case 0x95:
			Ins_sta(Address_zp_x_ptr());
			break;
		case 0x96:
			Ins_stx(Address_zp_y_ptr());
			break;
		case 0x97:
			Ins_extra_axs(Address_zp_y_ptr());
			break;
		case 0x98:
			Ins_tya();
			break;
		case 0x99:
            Ins_sta(Address_abs_y_ptr());
			break;
		case 0x9A:
			Ins_txs_x_sp(); // Tested
			break;
		case 0x9D:
			Ins_sta(Address_abs_x_ptr());
			break;
		case 0xA0:
			Ins_ldy(Address_imm_ptr()); // Tested
			break;
		case 0xA1:
			Ins_lda(Address_ind_x_ptr());
			break;
		case 0xA2:
			Ins_ldx(Address_imm_ptr()); // Tested
			break;
		case 0xA3:
			Ins_extra_lax(Address_ind_x_ptr()); // Tested
			break;
		case 0xA4:
			Ins_ldy(Address_zp_ptr());
			break;
		case 0xA5:
			Ins_lda(Address_zp_ptr()); // Tested
			break;
		case 0xA6:
			Ins_ldx(Address_zp_ptr()); // Tested
			break;
		case 0xA8:
			Ins_tay(); // Tested
			break;
		case 0xA7:
			Ins_extra_lax(Address_zp_ptr()); // Tested
			break;
		case 0xA9:
			Ins_lda(Address_imm_ptr()); // Tested
			break;
		case 0xAA:
			Ins_tax();
			break;
		case 0xAC:
			//std::cout << "Ldy abs " << (int) ReadMem((pc+2)) << (int) ReadMem((pc+1)) << std::endl;
			Ins_ldy(Address_abs_ptr()); // Tested
			break;
		case 0xAD:
			Ins_lda(Address_abs_ptr());
			break;
		case 0xAE:
			Ins_ldx(Address_abs_ptr());
			break;
		case 0xAF:
			Ins_extra_lax(Address_abs_ptr()); // Tested
			break;
		case 0xB0:
			Ins_bcs(ReadMem(++pc));
			break;
		case 0xB1:
			Ins_lda(Address_ind_y_ptr());
			break;
		case 0xB4:
			Ins_ldy(Address_zp_x_ptr());
			break;
		case 0xB3:
			Ins_extra_lax(Address_ind_y_ptr()); // Tested
			break;
		case 0xB5:
			Ins_lda(Address_zp_x_ptr()); // Tested
			break;
		case 0xB6:
			Ins_ldx(Address_zp_y_ptr());
			break;
		case 0xB7:
			Ins_extra_lax(Address_zp_y_ptr()); // Tested
			break;
		case 0xB8:
			Ins_clv();
			break;
		case 0xB9:
			Ins_lda(Address_abs_y_ptr()); // Tested
			break;
		case 0xBA:
			Ins_tsx();
			break;
		case 0xBC:
			Ins_ldy(Address_abs_x_ptr());
			break;
		case 0xBD:
			Ins_lda(Address_abs_x_ptr()); // Tested
			break;
		case 0xBE:
			Ins_ldx(Address_abs_x_ptr());
			break;
		case 0xBF:
			Ins_extra_lax(Address_abs_y_ptr()); // Tested
			break;
		case 0xC0:
			Ins_cpy(Address_imm_ptr()); // Tested
			break;
		case 0xC1:
            Ins_cmp(Address_ind_y_ptr());
			break;
		case 0xC2:
			Ins_extra_skb();
			break;
		case 0xC3:
			Ins_extra_dcm(Address_ind_x_ptr());
			break;
		case 0xC4:
			Ins_cpy(Address_zp_ptr());
			break;
		case 0xC5:
			Ins_cmp(Address_zp_ptr());
			break;
		case 0xC6:
			Ins_dec(Address_zp_ptr());
			break;
		case 0xC7:
			Ins_extra_dcm(Address_zp_ptr());
			break;
		case 0xC8:
			std::cout << "Inc Y" << std::endl;
			//Ins_inc_y(); // Tested
			Ins_inc(Address_y_ptr());
			break;
		case 0xC9:
			Ins_cmp(Address_imm_ptr());
			break;
		case 0xCA:
			Ins_dec(Address_x_ptr()); // Tested
			break;
		case 0xCC:
			Ins_cpy(Address_abs_ptr());
			break;
		case 0xCD:
			Ins_cmp(Address_abs_ptr());
			break;
		case 0xCE:
			Ins_dec(Address_abs_ptr());
			break;
		case 0xCF:
			Ins_extra_dcm(Address_abs_ptr());
			break;
		case 0xD0:
			//std::cout << "Branch If not Equal (!Zero Flag)" << std::endl;
			Ins_bne(ReadMem(++pc)); // Tested
			break;
		case 0xD1:
			Ins_cmp(Address_ind_y_ptr());
			break;
		case 0xD3:
			Ins_extra_dcm(Address_ind_y_ptr());
			break;
		case 0xD4:
			Ins_extra_skb();
			break;
		case 0xD5:
			Ins_cmp(Address_zp_ptr());
			break;
		case 0xD6:
			Ins_dec(Address_zp_x_ptr());
			break;
		case 0xD7:
			Ins_extra_dcm(Address_zp_x_ptr());
			break;
		case 0xD8:
			Ins_cld(); // Tested
			//std::cout << "Clear Decimal Flag" << std::endl;
			break;
		case 0xD9:
			Ins_cmp(Address_abs_y_ptr());
			break;
		case 0xDA:
			//#NOP
			break;
		case 0xDB:
			Ins_extra_dcm(Address_abs_y_ptr());
			break;
		case 0xDC:
			Ins_extra_skw();
			break;
		case 0xDD:
			Ins_cmp(Address_abs_x_ptr());
			break;
		case 0xDE:
			Ins_dec(Address_abs_x_ptr());
			break;
		case 0xDF:
			Ins_extra_dcm(Address_abs_x_ptr());
			break;
		case 0xE0:
			Ins_cpx(Address_imm_ptr());
			break;
		case 0xE1:
			Ins_sbc(Address_ind_x_ptr());
			break;
		case 0xE2:
			Ins_extra_skb();
			break;
		case 0xE3:
			Ins_extra_ins(Address_ind_x_ptr());
			break;
		case 0xE4:
			Ins_cpx(Address_zp_ptr());
			break;
		case 0xE5:
			Ins_sbc(Address_zp_ptr());
			break;
        case 0xE6:
        	std::cout << (int) ReadMem((pc+1)) << std::endl;
			Ins_inc(Address_zp_ptr());
        	std::cout << (int) ReadMem((pc)) << std::endl;
			break;
		case 0xE7:
			Ins_extra_ins(Address_zp_ptr());
			break;
		case 0xE8:
			Ins_inc(Address_x_ptr());
			break;
		case 0xE9:
			Ins_sbc(Address_imm_ptr());
			break;
		case 0xEA:
			//NOP
			//std::cout << "NOP" << std::endl;
			break;
		case 0xEB:
			Ins_sbc(Address_imm_ptr());
			break;
		case 0xEC:
			Ins_cpx(Address_abs_ptr());
			break;
		case 0xED:
			Ins_sbc(Address_abs_ptr());
			break;
		case 0xEE:
			Ins_inc(Address_abs_ptr());
			break;
		case 0xEF:
			Ins_extra_ins(Address_abs_ptr());
			break;
		case 0xF0:
			//std::cout << "Branh if Equal (zero Flag)" << std::endl;
			Ins_beq(ReadMem(++pc)); // Tested
			break;
		case 0xF1:
			Ins_sbc(Address_ind_y_ptr());
			break;
		case 0xF3:
			Ins_extra_ins(Address_ind_y_ptr());
			break;
		case 0xF4:
			Ins_extra_skb();
			break;
		case 0xF5:
			Ins_sbc(Address_zp_x_ptr());
			break;
		case 0xF6:
			Ins_inc(Address_zp_x_ptr());
			break;
		case 0xF7:
			Ins_extra_ins(Address_zp_x_ptr());
			break;
		case 0xF8:
			Ins_sed();
			break;
		case 0xF9:
			Ins_sbc(Address_abs_y_ptr());
			break;
		case 0xFA:
			//#NOP
			break;
		case 0xFB:
			Ins_extra_ins(Address_abs_y_ptr());
			break;
		case 0xFC:
			Ins_extra_skw();
			break;
		case 0xFD:
			Ins_sbc(Address_abs_x_ptr());
			break;
		case 0xFE:
			Ins_inc(Address_abs_x_ptr());
			break;
		case 0xFF:
			Ins_extra_ins(Address_abs_x_ptr());
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

void Emulator::Ins_jmp_ind(uint16_t address) {
	uint8_t lsb,msb;
	uint16_t address_lsb;
	uint16_t address_msb;
	lsb = ReadMem(address);
	std::cout << (int)address << std::endl;
	address_msb = address & 0xFF00;
	address_lsb = address & 0x00FF;
	std::cout << (int)address_msb << " " << (int)address_lsb << std::endl;

	address = address_msb | ((address_lsb +1)&0xFF);
	std::cout << (int)address << std::endl;
	msb = ReadMem(address);

    address = ((msb << 8 ) | lsb);
	pc = address-1;
}

void Emulator::Ins_ldx(uint8_t & value) {
	x = value;
	SetFlag((x & 0x80),FLAG_NEGATIVE);
	SetFlag(!x,FLAG_ZERO);
}

void Emulator::Ins_ldy(uint8_t & value) {
	y = value;
	SetFlag((y & 0x80),FLAG_NEGATIVE);
	SetFlag(!y,FLAG_ZERO);
}

void Emulator::Ins_lda(uint8_t & value) {
	ac = value;
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
	SetFlag(!ac,FLAG_ZERO);
}

void Emulator::Ins_adc(uint8_t& value)  //69", "IME", "SKIP", "SKIP")
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

void Emulator::Ins_and(uint8_t &value)  //29", "IME", "SKIP", "SKIP")
{
	std::cout << (int) value << std::endl;
	ac &= value;
	SetFlag((ac & 0x80),FLAG_NEGATIVE);
	SetFlag(!ac,FLAG_ZERO);
}

void Emulator::Ins_asl(uint8_t &src)  //0A", "SKIP", "SKIP", "SKIP")
{
	std::cout << "Pre  Shift Accumulator = " << std::bitset<8>(ac) << std::endl;
    SetFlag((src & 0x80),FLAG_CARRY);
    src <<= 1;
    src &= 0xFF;
	// std::cout << "Post Shift Accumulator = " << std::bitset<8>(ac) << std::endl;
    SetFlag((src & 0x80),FLAG_NEGATIVE);
    SetFlag(!src,FLAG_ZERO);
	// std::cout << "Status Register: SV BDIZC" << std::endl;
	// std::cout << "Status Register: " << std::bitset<8>(sr) << std::endl;

}

void Emulator::Ins_bit(uint8_t &src)  //24", "SKIP", "REG", "SKIP")
{
	std::cout << (int) src << std::endl;
	uint8_t tempValue = src;
	SetFlag((tempValue & 0x80), FLAG_NEGATIVE);
	SetFlag((tempValue & 0x40), FLAG_OVERFLOW);
    tempValue &= ac;
	std::cout << (int) src << std::endl;
    SetFlag((!tempValue), FLAG_ZERO);
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
	    // if (rel_address & 0x80) {
		   //  lsb -= (rel_address & 0x7F);
		   //  destination = ((msb << 8) | lsb);
	    // } else {
		   //  destination = ((msb << 8) | lsb) + rel_address;
	    // }
		destination = ((msb << 8) | (lsb + rel_address));
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

void Emulator::Ins_cmp(uint8_t & value)  //C9", "IME", "SKIP", "SKIP")
{
	std::cout << "Value: " << (int)value << std::endl;
	uint16_t result = ac - value;
	SetFlag(((result) < 0x100) , FLAG_CARRY);
	SetFlag((result & 0x80) , FLAG_NEGATIVE);
	SetFlag((!result) , FLAG_ZERO);
}

void Emulator::Ins_cpx(uint8_t & value)  //E0", "IME", "SKIP", "SKIP")
{
	uint16_t result = x - value;
	SetFlag((result < 0x100) , FLAG_CARRY);
	SetFlag((result & 0x80) , FLAG_NEGATIVE);
	SetFlag((!result) , FLAG_ZERO);}

void Emulator::Ins_cpy(uint8_t & value)  //C0", "IME", "SKIP", "SKIP")
{
	uint16_t result = y - value;
	SetFlag((result < 0x100) , FLAG_CARRY);
	SetFlag((result & 0x80) , FLAG_NEGATIVE);
	SetFlag((!result) , FLAG_ZERO);
}

void Emulator::Ins_eor(uint8_t &value)  //49", "IME", "SKIP", "SKIP")
{
	ac ^= value;
	SetFlag((ac & 0x80) , FLAG_NEGATIVE);
	SetFlag((ac == 0) , FLAG_ZERO);
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

void Emulator::Ins_ora(uint8_t & value)  //09", "IME", "SKIP", "SKIP")
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
	std::cout << (int) tempValue << std::endl;

    tempValue <<= 1;
	std::cout << (int) tempValue << std::endl;
    if (TestFlag(FLAG_CARRY)) {
    	tempValue |= 0x01;
    }
    SetFlag((tempValue > 0xFF), FLAG_CARRY);
    tempValue &= 0xFF;
	std::cout << (int) tempValue << std::endl;
    SetFlag((tempValue & 0x80), FLAG_NEGATIVE);
    SetFlag((!tempValue), FLAG_ZERO);
    src = tempValue;
}

void Emulator::Ins_ror(uint8_t & src)  //6A", "SKIP", "SKIP", "SKIP")
{
	uint16_t tempValue = src;
    if (TestFlag(FLAG_CARRY)){
		tempValue |= 0x100;
	}
    SetFlag((tempValue & 0x01),FLAG_CARRY);
    tempValue >>= 1;
	SetFlag((tempValue & 0x80), FLAG_NEGATIVE);
    SetFlag(!tempValue, FLAG_ZERO);
    src = tempValue;
}

void Emulator::Ins_rti()  //40", "SKIP", "SKIP", "SKIP")
{
	sr = StackPop();
	sr |= 0x20;
	pc = StackPop();
	pc |= (StackPop() << 8);
	pc--;
}

void Emulator::Ins_sbc(uint8_t & value)  //E9", "IME", "SKIP", "SKIP")
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

void Emulator::Ins_sta(uint8_t & value)  //8D", "SKIP", "REG", "SKIP")
{
	//std::cout << "Storing AC: " << ac << " In address "	<< address << std::endl;
    //WriteMem(address, ac);
    value = ac;
}

void Emulator::Ins_stx(uint8_t & value)  //8D", "SKIP", "REG", "SKIP")
{
	//std::cout << "Storing AC: " << ac << " In address "	<< address << std::endl;
    //WriteMem(address, ac);
    value = x;
}


void Emulator::Ins_sty(uint8_t & value)  //8D", "SKIP", "REG", "SKIP")
{
	//std::cout << "Storing AC: " << ac << " In address "	<< address << std::endl;
    //WriteMem(address, ac);
    value = y;
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


void Emulator::Ins_extra_skb() {
	pc++;
}

void Emulator::Ins_extra_skw() {
	pc++;
	pc++;
}

void Emulator::Ins_extra_lax(uint8_t & value) {
	ac = value;
	x = value;
	SetFlag((value & 0x80),FLAG_NEGATIVE);
	SetFlag(!value,FLAG_ZERO);
}

void Emulator::Ins_extra_axs(uint8_t & value) {
	uint8_t tempReg = sr;
	Ins_stx(value);
	Ins_pha();
	Ins_and(value);
	Ins_sta(value);
	Ins_pla();
	sr = tempReg;
}

void Emulator::Ins_extra_dcm(uint8_t & value) {
	Ins_dec(value);
	Ins_cmp(value);
}

void Emulator::Ins_extra_ins(uint8_t & value) {
	Ins_inc(value);
	Ins_sbc(value);
}

void Emulator::Ins_extra_aso(uint8_t & value) {
	Ins_asl(value);
	Ins_ora(value);
}

void Emulator::Ins_extra_rla(uint8_t & value) {
	Ins_rol(value);
	Ins_and(value);
}

void Emulator::Ins_extra_lse(uint8_t & value) {
	Ins_lsr(value);
	Ins_eor(value);
}

void Emulator::Ins_lsr(uint8_t & src)  //4A", "SKIP", "SKIP", "SKIP")
{
	SetFlag((src & 0x01), FLAG_CARRY);
	src >>= 1;
	SetFlag(0, FLAG_NEGATIVE);
	SetFlag((!src), FLAG_ZERO);
}

void Emulator::Ins_extra_rra(uint8_t & value)
{
	std::cout << "Value: " << (int) value << std::endl;
	Ins_ror(value);
	Ins_adc(value);
}


// C7E9 Check Ac 6f != 7f // so  the 0x20 flag?
// C800 Check Ac 64 != 74 // 0x68 opcode
// C817 Check Ac 2f != 3f // 0x68 opcode
