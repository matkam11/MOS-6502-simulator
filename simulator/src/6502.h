/*





*/
#ifndef LIB_CPP_6502_H
#define LIB_CPP_6502_H

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
#include "String.h"

// set if memory was modified during processing of the last instruction
#define EMU_FLAG_DIRTY 0x01
// set if the emulator should wait for an interrupt before continuing
#define EMU_FLAG_WAIT_FOR_INTERRUPT 0x02
class Emulator {

/*Singleton approach for this class-------------------------------------------*/
public:
    static Emulator& getInstance()
    {
        static Emulator instance;
        return instance;
    }

    void Reset();

private:
    Emulator();
    Emulator(Emulator const&);
    void operator=(Emulator const&);

    Emulator(uint16_t pc_start);

public:
    //
    // Static fields
    //

    // Base address to load memory
    static uint16_t base_addr;

    // Base address to start execution
    static uint16_t start_addr;

    // Show emu help message
    static bool help;

    // Message to display with '--emu-help'
    static const std::string help_message;


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

    uint16_t ReadTwoBytes();
    bool Decode();
    void SetFlag(bool set, uint8_t Flag);
    void MemoryWatch(uint16_t address);

    uint16_t GetStartAddr();
    uint16_t GetBaseAddr();
    void SetStartAddr(const uint16_t &pc_start);
    uint16_t GetPC();
    void SetPC(const uint16_t &pc_pos);

    bool TestFlag(uint8_t Flag);

    void StackPush(uint8_t byte);
    uint8_t StackPop();
    void PrintStack();
    void PrintMem();

    void WriteMem(uint16_t address, uint8_t value);
    uint8_t ReadMem(uint16_t address);

    // Command line options for EMULATOR
    void RegisterOptions();
    void ProcessOptions();

    uint8_t inline & Address_acc_ptr();
    uint8_t inline & Address_x_ptr();
    uint8_t inline & Address_y_ptr();
    uint8_t inline & Address_imm_ptr();
    uint8_t inline & Address_zp_ptr();
    uint8_t inline & Address_zp_x_ptr();
    uint8_t inline & Address_zp_y_ptr();
    uint8_t inline & Address_abs_ptr();
    uint8_t inline & Address_abs_x_ptr();
    uint8_t inline & Address_abs_y_ptr();
    uint8_t inline & Address_ind_x_ptr();
    uint8_t inline & Address_ind_y_ptr();

    void Ins_jsr(uint16_t destination);
    void Ins_rts();
    void Ins_jmp_abs(uint16_t destination);
    void Ins_jmp_ind(uint16_t address);


    void Ins_ldx(uint8_t & value);
    void Ins_ldy(uint8_t & value);
    void Ins_lda(uint8_t & value);

    void Ins_adc(uint8_t &value);  // "0x69", "IME", "SKIP", "SKIP");
    void Ins_and(uint8_t &value);  // "0x29", "IME", "SKIP", "SKIP");
    void Ins_asl(uint8_t &src);  // "0x0A", "SKIP", "SKIP", "SKIP");
    // void Ins_asl_acc();  // "0x0A", "SKIP", "SKIP", "SKIP");
    // void Ins_asl_zp(uint8_t zero_addr);  // "0x06", "SKIP", "REG", "SKIP");
    void ExecuteInst_asl_zp_x();  // "0x16", "SKIP", "REG", "OFFS");
    void Ins_asl_abs(uint16_t address);  // "0x0E", "SKIP", "REG", "SKIP");
    void Ins_asl_abs_x(uint16_t abs_addr);  // "0x1E", "SKIP", "REG", "OFFS");
    void Ins_bit(uint8_t &src);  // "0x24", "SKIP", "REG", "SKIP");
    void Ins_bpl(uint8_t rel_address);  // "0x10", "SKIP", "SKIP", "SKIP");
    void Ins_bmi(uint8_t rel_address);  // "0x30", "SKIP", "SKIP", "SKIP");
    void Ins_bvc(uint8_t rel_address);  // "0x50", "SKIP", "SKIP", "SKIP");
    void Ins_bvs(uint8_t rel_address);  // "0x70", "SKIP", "SKIP", "SKIP");
    void Ins_bcc(uint8_t rel_address);  // "0x90", "SKIP", "SKIP", "SKIP");
    void Ins_bcs(uint8_t rel_address);  // "0xB0", "SKIP", "SKIP", "SKIP");
    void Ins_bne(uint8_t rel_address);  // "0xD0", "SKIP", "SKIP", "SKIP");
    void Ins_beq(uint8_t rel_address);  // "0xF0", "SKIP", "SKIP", "SKIP");
    void Ins_cmp(uint8_t & value);  // "0xC9", "IME", "SKIP", "SKIP");
    void Ins_cpx(uint8_t & value);  // "0xE0", "IME", "SKIP", "SKIP");
    void Ins_cpy(uint8_t & value);  // "0xC0", "IME", "SKIP", "SKIP");
    void Ins_eor(uint8_t & value);  // "0x49", "IME", "SKIP", "SKIP");
    void Ins_clc();  // "0x18", "SKIP", "SKIP", "SKIP");
    void Ins_sec();  // "0x38", "SKIP", "SKIP", "SKIP");
    void Ins_cli();  // "0x58", "SKIP", "SKIP", "SKIP");
    void Ins_sei();  // "0x78", "SKIP", "SKIP", "SKIP");
    void Ins_clv();  // "0xB8", "SKIP", "SKIP", "SKIP");
    void Ins_cld();  // "0xD8", "SKIP", "SKIP", "SKIP");
    void Ins_sed();  // "0xD8", "SKIP", "SKIP", "SKIP");
    void Ins_inc(uint8_t &src);  // "0xE6", "SKIP", "REG", "SKIP");
    void Ins_lsr(uint8_t & src);  // "0x4A", "SKIP", "SKIP", "SKIP");
    void Ins_ora(uint8_t & value);  // "0x09", "IME", "SKIP", "SKIP");
    void Ins_tax();  // "0xAA", "SKIP", "SKIP", "SKIP");
    void Ins_txa();  // "0x8A", "SKIP", "SKIP", "SKIP");
    void Ins_dec(uint8_t &src);  // "0xCA", "SKIP", "SKIP", "SKIP");
    void Ins_tay();  // "0xA8", "SKIP", "SKIP", "SKIP");
    void Ins_tya();  // "0x98", "SKIP", "SKIP", "SKIP");
    void Ins_rol(uint8_t & src);
    void Ins_ror(uint8_t & value);  // "0x6A", "SKIP", "SKIP", "SKIP");
    void Ins_rti();  // "0x40", "SKIP", "SKIP", "SKIP");
    void Ins_sbc(uint8_t & value);  // "0xE9", "IME", "SKIP", "SKIP");
    void Ins_sta(uint8_t & address);  // "0x8D", "SKIP", "REG", "SKIP");
    void Ins_stx(uint8_t & address);  // "0x8D", "SKIP", "REG", "SKIP");
    void Ins_sty(uint8_t & address);  // "0x8D", "SKIP", "REG", "SKIP");   
    void Ins_txs_x_sp();  // "0x9A", "SKIP", "SKIP", "SKIP");
    void Ins_tsx();  // "0xBA", "SKIP", "SKIP", "SKIP");
    void Ins_pha();  // "0x48", "SKIP", "SKIP", "SKIP");
    void Ins_pla();  // "0x68", "SKIP", "SKIP", "SKIP");
    void Ins_php();  // "0x08", "SKIP", "SKIP", "SKIP");
    void Ins_plp();  // "0x28", "SKIP", "SKIP", "SKIP");

    void Ins_extra_skb();
    void Ins_extra_skw();
    void Ins_extra_rra(uint8_t & value);
    void Ins_extra_lax(uint8_t & value);
    void Ins_extra_axs(uint8_t & value);
    void Ins_extra_dcm(uint8_t & value);
    void Ins_extra_ins(uint8_t & value);
    void Ins_extra_aso(uint8_t & value);
    void Ins_extra_rla(uint8_t & value);
    void Ins_extra_lse(uint8_t & value);
};

#endif
        // void ExecuteInst_lda_imm();  // "0xA9", "IME", "SKIP", "SKIP");
        // void ExecuteInst_lda_zp();  // "0xA5", "SKIP", "REG", "SKIP");
        // void ExecuteInst_lda_zp_x();  // "0xB5", "SKIP", "REG", "OFFS");
        // void ExecuteInst_lda_abs();  // "0xAD", "SKIP", "REG", "SKIP");
        // void ExecuteInst_lda_abs_x();  // "0xBD", "SKIP", "REG", "OFFS");
        // void ExecuteInst_lda_abs_y();  // "0xB9", "SKIP", "REG", "OFFS");
        // void ExecuteInst_lda_ind_x();  // "0xA1", "SKIP", "REG", "OFFS");
        // void ExecuteInst_lda_ind_y();  // "0xB1", "SKIP", "REG", "OFFS");
        // void ExecuteInst_ldx_imm();  // "0xA2", "IME", "SKIP", "SKIP");
        // void ExecuteInst_ldx_zp();  // "0xA6", "SKIP", "REG", "SKIP");
        // void ExecuteInst_ldx_zp_x();  // "0xB6", "SKIP", "REG", "OFFS");
        // void ExecuteInst_ldx_abs();  // "0xAE", "SKIP", "REG", "SKIP");
        // void ExecuteInst_ldx_abs_x();  // "0xBE", "SKIP", "REG", "OFFS");
        // void ExecuteInst_ldy_imm();  // "0xA0", "IME", "SKIP", "SKIP");
        // void ExecuteInst_ldy_zp();  // "0xA4", "SKIP", "REG", "SKIP");
        // void ExecuteInst_ldy_zp_x();  // "0xB4", "SKIP", "REG", "OFFS");
        // void ExecuteInst_ldy_abs();  // "0xAC", "SKIP", "REG", "SKIP");
        // void ExecuteInst_ldy_abs_x();  // "0xBC", "SKIP", "REG", "OFFS");
