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
        uint8_t inline & Address_zp_ptr(uint8_t zero_addr);
        uint8_t inline & Address_zp_x_ptr(uint8_t zero_addr);
        uint8_t inline & Address_abs_ptr(uint16_t address);
        uint8_t inline & Address_abs_x_ptr(uint16_t address);
        uint8_t inline Address_zp(uint8_t zero_addr);
        uint8_t inline Address_zp_x(uint8_t zero_addr);
        uint8_t inline Address_zp_y(uint8_t zero_addr);
        uint8_t inline Address_abs(uint16_t address);
        uint8_t inline Address_abs_x(uint16_t address);
        uint8_t inline Address_abs_y(uint16_t address);
        uint8_t inline Address_ind_x(uint8_t start_address);
        uint8_t inline Address_ind_y(uint8_t start_address);

    void Ins_jsr(uint16_t destination);
    void Ins_rts();
    void Ins_jmp_abs(uint16_t destination);

    void Ins_ldx_imm(uint8_t value);
    void Ins_ldy_imm(uint8_t value);
    void Ins_lda_imm(uint8_t value);

    void Ins_lda_ind_x(uint8_t start_address);

        void Ins_adc_imm(uint8_t value);  // "0x69", "IME", "SKIP", "SKIP");
        void ExecuteInst_adc_zp();  // "0x65", "SKIP", "REG", "SKIP");
        void ExecuteInst_adc_zp_x();  // "0x75", "SKIP", "REG", "OFFS");
        void ExecuteInst_adc_abs();  // "0x6D", "SKIP", "REG", "SKIP");
        void ExecuteInst_adc_abs_x();  // "0x7D", "SKIP", "REG", "OFFS");
        void ExecuteInst_adc_abs_y();  // "0x79", "SKIP", "REG", "OFFS");
        void Ins_adc_ind_x(uint8_t start_address);  // "0x61", "SKIP", "REG", "OFFS");
        void ExecuteInst_adc_ind_y();  // "0x71", "SKIP", "REG", "OFFS");
        void Ins_and_imm(uint8_t value);  // "0x29", "IME", "SKIP", "SKIP");
        void ExecuteInst_and_zp();  // "0x25", "SKIP", "REG", "SKIP");
        void ExecuteInst_and_zp_x();  // "0x35", "SKIP", "REG", "OFFS");
        void ExecuteInst_and_abs();  // "0x2D", "SKIP", "REG", "SKIP");
        void ExecuteInst_and_abs_x();  // "0x3D", "SKIP", "REG", "OFFS");
        void ExecuteInst_and_abs_y();  // "0x39", "SKIP", "REG", "OFFS");
        void Ins_and_ind_x(uint8_t start_address);  // "0x21", "SKIP", "REG", "OFFS");
        void ExecuteInst_and_ind_y();  // "0x31", "SKIP", "REG", "OFFS");
        void Ins_asl(uint8_t &src);  // "0x0A", "SKIP", "SKIP", "SKIP");
        // void Ins_asl_acc();  // "0x0A", "SKIP", "SKIP", "SKIP");
        // void Ins_asl_zp(uint8_t zero_addr);  // "0x06", "SKIP", "REG", "SKIP");
        void ExecuteInst_asl_zp_x();  // "0x16", "SKIP", "REG", "OFFS");
        void Ins_asl_abs(uint16_t address);  // "0x0E", "SKIP", "REG", "SKIP");
        void Ins_asl_abs_x(uint16_t abs_addr);  // "0x1E", "SKIP", "REG", "OFFS");
        void Ins_bit_zp(uint8_t zero_addr);  // "0x24", "SKIP", "REG", "SKIP");
        void ExecuteInst_bit_abs();  // "0x2C", "SKIP", "REG", "SKIP");
        void Ins_bpl(uint8_t rel_address);  // "0x10", "SKIP", "SKIP", "SKIP");
        void Ins_bmi(uint8_t rel_address);  // "0x30", "SKIP", "SKIP", "SKIP");
        void Ins_bvc(uint8_t rel_address);  // "0x50", "SKIP", "SKIP", "SKIP");
        void Ins_bvs(uint8_t rel_address);  // "0x70", "SKIP", "SKIP", "SKIP");
        void Ins_bcc(uint8_t rel_address);  // "0x90", "SKIP", "SKIP", "SKIP");
        void Ins_bcs(uint8_t rel_address);  // "0xB0", "SKIP", "SKIP", "SKIP");
        void Ins_bne(uint8_t rel_address);  // "0xD0", "SKIP", "SKIP", "SKIP");
        void Ins_beq(uint8_t rel_address);  // "0xF0", "SKIP", "SKIP", "SKIP");
        void ExecuteInst_brk();  // "0x00", "SKIP", "SKIP", "SKIP");
        void Ins_cmp_imm(uint8_t value);  // "0xC9", "IME", "SKIP", "SKIP");
        void Ins_cmp_zp(uint8_t zero_addr);  // "0xC5", "SKIP", "REG", "SKIP");
        void ExecuteInst_cmp_zp_x();  // "0xD5", "SKIP", "REG", "OFFS");
        void Ins_cmp_abs(uint16_t address);  // "0xCD", "SKIP", "REG", "SKIP");
        void ExecuteInst_cmp_abs_x();  // "0xDD", "SKIP", "REG", "OFFS");
        void ExecuteInst_cmp_abs_y();  // "0xD9", "SKIP", "REG", "OFFS");
        void Ins_cmp_ind_x(uint8_t start_address);  // "0xC1", "SKIP", "REG", "OFFS");
        void ExecuteInst_cmp_ind_y();  // "0xD1", "SKIP", "REG", "OFFS");
        void Ins_cpx_imm(uint8_t value);  // "0xE0", "IME", "SKIP", "SKIP");
        void ExecuteInst_cpx_zp();  // "0xE4", "SKIP", "REG", "SKIP");
        void ExecuteInst_cpx_abs();  // "0xEC", "SKIP", "REG", "SKIP");
        void Ins_cpy_imm(uint8_t value);  // "0xC0", "IME", "SKIP", "SKIP");
        void ExecuteInst_cpy_zp();  // "0xC4", "SKIP", "REG", "SKIP");
        void ExecuteInst_cpy_abs();  // "0xCC", "SKIP", "REG", "SKIP");
        void ExecuteInst_dec_zp();  // "0xC6", "SKIP", "REG", "SKIP");
        void ExecuteInst_dec_zp_x();  // "0xD6", "SKIP", "REG", "OFFS");
        void ExecuteInst_dec_abs();  // "0xCE", "SKIP", "REG", "SKIP");
        void ExecuteInst_dec_abs_x();  // "0xDE", "SKIP", "REG", "OFFS");
        void Ins_eor_imm(uint8_t value);  // "0x49", "IME", "SKIP", "SKIP");
        void ExecuteInst_eor_zp();  // "0x45", "SKIP", "REG", "SKIP");
        void ExecuteInst_eor_zp_x();  // "0x55", "SKIP", "REG", "OFFS");
        void ExecuteInst_eor_abs();  // "0x4D", "SKIP", "REG", "SKIP");
        void ExecuteInst_eor_abs_x();  // "0x5D", "SKIP", "REG", "OFFS");
        void ExecuteInst_eor_abs_y();  // "0x59", "SKIP", "REG", "OFFS");
        void Ins_eor_ind_x(uint8_t start_address);  // "0x41", "SKIP", "REG", "OFFS");
        void ExecuteInst_eor_ind_y();  // "0x51", "SKIP", "REG", "OFFS");
        void Ins_clc();  // "0x18", "SKIP", "SKIP", "SKIP");
        void Ins_sec();  // "0x38", "SKIP", "SKIP", "SKIP");
        void Ins_cli();  // "0x58", "SKIP", "SKIP", "SKIP");
        void Ins_sei();  // "0x78", "SKIP", "SKIP", "SKIP");
        void Ins_clv();  // "0xB8", "SKIP", "SKIP", "SKIP");
        void Ins_cld();  // "0xD8", "SKIP", "SKIP", "SKIP");
        void Ins_sed();  // "0xD8", "SKIP", "SKIP", "SKIP");
        void Ins_inc(uint8_t &src);  // "0xE6", "SKIP", "REG", "SKIP");
        void ExecuteInst_inc_zp_x();  // "0xF6", "SKIP", "REG", "OFFS");
        void ExecuteInst_inc_abs();  // "0xEE", "SKIP", "REG", "SKIP");
        void ExecuteInst_inc_abs_x();  // "0xFE", "SKIP", "REG", "OFFS");
        void ExecuteInst_jmp_abs();  // "0x4C", "IME", "SKIP", "SKIP");
        void ExecuteInst_jmp_ind();  // "0x6C", "IME", "SKIP", "SKIP");
        void ExecuteInst_jsr_abs();  // "0x20", "IME", "SKIP", "SKIP");
        void Ins_lsr_acc();  // "0x4A", "SKIP", "SKIP", "SKIP");
        void Ins_lsr_zp(uint8_t zero_address);  // "0x46", "SKIP", "REG", "SKIP");
        void ExecuteInst_lsr_zp_x();  // "0x56", "SKIP", "REG", "OFFS");
        void ExecuteInst_lsr_abs();  // "0x4E", "SKIP", "REG", "SKIP");
        void ExecuteInst_lsr_abs_x();  // "0x5E", "SKIP", "REG", "OFFS");
        void Ins_ora_imm(uint8_t value);  // "0x09", "IME", "SKIP", "SKIP");
        // void ExecuteInst_ora_zp();  // "0x05", "SKIP", "REG", "SKIP");
        void ExecuteInst_ora_zp_x();  // "0x15", "SKIP", "REG", "OFFS");
        void ExecuteInst_ora_abs();  // "0x0D", "SKIP", "REG", "SKIP");
        void ExecuteInst_ora_abs_x();  // "0x1D", "SKIP", "REG", "OFFS");
        void ExecuteInst_ora_abs_y();  // "0x19", "SKIP", "REG", "OFFS");
        void Ins_ora_ind_x(uint8_t start_address);  // "0x01", "SKIP", "REG", "OFFS");
        void ExecuteInst_ora_ind_y();  // "0x11", "SKIP", "REG", "OFFS");
        void Ins_tax();  // "0xAA", "SKIP", "SKIP", "SKIP");
        void Ins_txa();  // "0x8A", "SKIP", "SKIP", "SKIP");
        void Ins_dec(uint8_t &src);  // "0xCA", "SKIP", "SKIP", "SKIP");
        void Ins_tay();  // "0xA8", "SKIP", "SKIP", "SKIP");
        void Ins_tya();  // "0x98", "SKIP", "SKIP", "SKIP");
        void Ins_rol(uint8_t &src);
        void Ins_rol_acc();  // "0x2A", "SKIP", "SKIP", "SKIP");
        void Ins_rol_zp(uint8_t zero_address);  // "0x26", "SKIP", "REG", "SKIP");
        void ExecuteInst_rol_zp_x();  // "0x36", "SKIP", "REG", "OFFS");
        void ExecuteInst_rol_abs();  // "0x2E", "SKIP", "REG", "SKIP");
        void ExecuteInst_rol_abs_x();  // "0x3E", "SKIP", "REG", "OFFS");
        void Ins_ror_acc();  // "0x6A", "SKIP", "SKIP", "SKIP");
        void Ins_ror_zp(uint8_t zero_address);  // "0x66", "SKIP", "REG", "SKIP");
        void ExecuteInst_ror_zp_x();  // "0x76", "SKIP", "REG", "OFFS");
        void Ins_ror_abs(uint16_t address);  // "0x6E", "SKIP", "REG", "SKIP");
        void ExecuteInst_ror_abs_x();  // "0x7E", "SKIP", "REG", "OFFS");
        void Ins_rti();  // "0x40", "SKIP", "SKIP", "SKIP");
        void ExecuteInst_rts();  // "0x60", "SKIP", "SKIP", "SKIP");
        void Ins_sbc_imm(uint8_t value);  // "0xE9", "IME", "SKIP", "SKIP");
        void ExecuteInst_sbc_zp();  // "0xE5", "SKIP", "REG", "SKIP");
        void ExecuteInst_sbc_zp_x();  // "0xF5", "SKIP", "REG", "OFFS");
        void ExecuteInst_sbc_abs();  // "0xED", "SKIP", "REG", "SKIP");
        void ExecuteInst_sbc_abs_x();  // "0xFD", "SKIP", "REG", "OFFS");
        void ExecuteInst_sbc_abs_y();  // "0xF9", "SKIP", "REG", "OFFS");
        void Ins_sbc_ind_x(uint8_t start_address);  // "0xE1", "SKIP", "REG", "OFFS");
        void ExecuteInst_sbc_ind_y();  // "0xF1", "SKIP", "REG", "OFFS");
        void Ins_sta_zp(uint8_t zero_addr);  // "0x85", "SKIP", "REG", "SKIP");
        void ExecuteInst_sta_zp_x();  // "0x95", "SKIP", "REG", "OFFS");
        void Ins_sta_abs(uint16_t address);  // "0x8D", "SKIP", "REG", "SKIP");
        void Ins_sta_abs_x(uint16_t address);  // "0x9D", "SKIP", "REG", "OFFS");
        void ExecuteInst_sta_abs_y();  // "0x99", "SKIP", "REG", "OFFS");
        void Ins_sta_ind_x(uint8_t start_address);  // "0x81", "SKIP", "REG", "OFFS");
        void Ins_sta_ind_y(uint8_t start_address);  // "0x91", "SKIP", "REG", "OFFS");
        void Ins_txs_x_sp();  // "0x9A", "SKIP", "SKIP", "SKIP");
        void Ins_tsx();  // "0xBA", "SKIP", "SKIP", "SKIP");
        void Ins_pha();  // "0x48", "SKIP", "SKIP", "SKIP");
        void Ins_pla();  // "0x68", "SKIP", "SKIP", "SKIP");
        void Ins_php();  // "0x08", "SKIP", "SKIP", "SKIP");
        void Ins_plp();  // "0x28", "SKIP", "SKIP", "SKIP");
        void Ins_stx_zp(uint8_t address);  // "0x86", "SKIP", "REG", "SKIP");
        void ExecuteInst_stx_zp_x();  // "0x96", "SKIP", "REG", "OFFS");
        void Ins_stx_abs(uint16_t address);  // "0x8e", "SKIP", "REG", "SKIP");
        void Ins_sty_zp(uint8_t address);  // "0x84", "SKIP", "REG", "SKIP");
        void ExecuteInst_sty_zp_x();  // "0x94", "SKIP", "REG", "OFFS");
        void Ins_sty_abs(uint16_t address);  // "0x8C", "SKIP", "REG", "SKIP");

        void Ins_extra_rra_zp_x(uint8_t zero_addr);

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
