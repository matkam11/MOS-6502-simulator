#include <lib/cpp/Misc.h>
#include "Context.h"

namespace mos6502
{ 

void Context::ExecuteInst_adc_imm("0x69", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_adc_zp("0x65", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_adc_zp_x("0x75", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_adc_abs("0x6D", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_adc_abs_x("0x7D", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_adc_abs_y("0x79", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_adc_ind_x("0x61", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_adc_ind_y("0x71", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_and_imm("0x29", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_and_zp("0x25", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_and_zp_x("0x35", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_and_abs("0x2D", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_and_abs_x("0x3D", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_and_abs_y("0x39", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_and_ind_x("0x21", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_and_ind_y("0x31", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_asl_acc("0x0A", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_asl_zp("0x06", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_asl_zp_x("0x16", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_asl_abs("0x0E", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_asl_abs_x("0x11", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_bit_zp("0x24", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_bit_abs("0x2C", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_br_on_pl("0x10", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_br_on_mi("0x30", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_br_on_vc("0x50", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_br_on_vs("0x70", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_br_on_cc("0x90", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_br_on_cs("0xB0", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_br_on_ne("0xD0", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_br_on_eq("0xF0", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_brk("0x00", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cmp_imm("0xC9", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cmp_zp("0xC5", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cmp_zp_x("0xD5", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cmp_abs("0xCD", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cmp_abs_x("0xDD", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cmp_abs_y("0xD9", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cmp_ind_x("0xC1", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cmp_ind_y("0xD1", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cpx_imm("0xE0", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cpx_zp("0xE4", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cpx_abs("0xEC", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cpy_imm("0xC0", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cpy_zp("0xC4", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_cpy_abs("0xCC", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_dec_zp("0xC6", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_dec_zp_x("0xD6", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_dec_abs("0xCE", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_dec_abs_x("0xDE", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_eor_imm("0x49", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_eor_zp("0x45", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_eor_zp_x("0x55", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_eor_abs("0x4D", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_eor_abs_x("0x5D", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_eor_abs_y("0x59", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_eor_ind_x("0x41", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_eor_ind_y("0x51", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_flag_clc("0x18", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_flag_sec("0x38", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_flag_cli("0x58", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_flag_sei("0x78", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_flag_clv("0xB8", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_flag_cld("0xC8", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_flag_sed("0xD8", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_inc_zp("0xE6", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_inc_zp_x("0xF6", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_inc_abs("0xEE", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_inc_abs_x("0xFE", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_jmp_abs("0x4C", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_jmp_ind("0x6C", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_jsr_abs("0x20", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_lda_imm("0xA9", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_lda_zp("0xA5", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_lda_zp_x("0xB5", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_lda_abs("0xAD", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_lda_abs_x("0xBD", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_lda_abs_y("0xB9", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_lda_ind_x("0xA1", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_lda_ind_y("0xB1", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ldx_imm("0xA2", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ldx_zp("0xA6", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ldx_zp_x("0xB6", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ldx_abs("0xAE", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ldx_abs_x("0xBE", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ldy_imm("0xA0", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ldy_zp("0xA4", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ldy_zp_x("0xB4", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ldy_abs("0xAC", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ldy_abs_x("0xBC", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_lsr_acc("0x4A", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_lsr_zp("0x46", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_lsr_zp_x("0x56", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_lsr_abs("0x4E", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_lsr_abs_x("0x5E", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ora_imm("0x09", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ora_zp("0x05", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ora_zp_x("0x15", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ora_abs("0x0D", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ora_abs_x("0x1D", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ora_abs_y("0x19", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ora_ind_x("0x01", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ora_ind_y("0x11", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_tax_a_x("0xAA", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_txa_x_a("0x8A", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_dex_x("0xCA", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_inx_x("0xE8", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_tay_a_y("0xA8", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_tya_y_a("0x98", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_dey_y("0x88", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_iny_y("0xC8", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_rol_acc("0x2A", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_rol_zp("0x26", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_rol_zp_x("0x36", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_rol_abs("0x2E", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_rol_abs_x("0x3E", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ror_acc("0x6A", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ror_zp("0x66", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ror_zp_x("0x76", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ror_abs("0x6E", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_ror_abs_x("0x7E", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_rti("0x40", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_rts("0x60", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sbc_imm("0xE9", "IME", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sbc_zp("0xE5", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sbc_zp_x("0xF5", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sbc_abs("0xED", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sbc_abs_x("0xFD", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sbc_abs_y("0xF9", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sbc_ind_x("0xE1", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sbc_ind_y("0xF1", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sta_zp("0x85", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sta_zp_x("0x95", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sta_abs("0x8D", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sta_abs_x("0x9D", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sta_abs_y("0x99", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sta_ind_x("0x81", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sta_ind_y("0x91", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_txs_x_sp("0x9A", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_tsx_sp_x("0xBA", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_pha_x_sp("0x48", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_pla_x_sp("0x68", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_php_x_sp("0x08", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_plp_x_sp("0x28", "SKIP", "SKIP", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sty_zp("0x86", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sty_zp_x("0x96", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sty_abs("0x8e", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sty_zp("0x84", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sty_zp_x("0x94", "SKIP", "REG", "OFFS")
{
	throw misc::Panic("Unimplemented instruction");
}


void Context::ExecuteInst_sty_abs("0x8C", "SKIP", "REG", "SKIP")
{
	throw misc::Panic("Unimplemented instruction");
}



}  // namespace mos6502
