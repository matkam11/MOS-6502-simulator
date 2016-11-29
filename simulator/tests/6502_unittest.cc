#include "gtest/gtest.h"
#include "6502.h"
//#include "Misc.h"
// Test creating 6502 object
TEST(emulator_object_test, Suceess) {
  // This test is named "Negative", and belongs to the "FactorialTest"
  // test case.
	uint16_t base_addr = 20;
    Emulator emu(base_addr);
    EXPECT_EQ(20, emu.pc);
}
// Test that value is set correctly


TEST(emulator_meta, Memory) {
  // This test is named "Negative", and belongs to the "FactorialTest"
  // test case.
	uint16_t base_addr = 0;
    Emulator emu(base_addr);
    emu.WriteMem(0x1000, 0x10);
    EXPECT_EQ(0xFF, emu.ReadMem(0x0000));
    EXPECT_EQ(0x10, emu.ReadMem(0x1000));
}

TEST(emulator_meta, Stack) {
	uint16_t base_addr = 0;
    Emulator emu(base_addr);

    // Test Memory with Stack. Also tests Stack Logic
    // Empty Stack
    // [0xFF .. 0xFF 0xFF 0xFF]
    //                    SP ^
    // Push AB
    // [0xFF .. 0xFF 0xFF 0xAB]
    //               SP ^
    // Push BC
    // [0xFF .. 0xFF 0xBC 0xAB]
    //          SP ^
    // Pop BC, Returns BC
    // [0xFF .. 0xFF 0xBC 0xAB]
    //				 SP ^
    // Push CD
    // [0xFF .. 0xFF 0xCD 0xAB]
    //			SP ^	 	 
    // Pop CD, Returns CD
    // [0xFF .. 0xFF 0xCD 0xAB]
    //				 SP ^
    // Pop AB, Retruns AB
    // [0xFF .. 0xFF 0xCD 0xAB]
    //			          SP ^	 	 
    EXPECT_EQ(0xFF, emu.sp);
    emu.StackPush(0xAB);
    EXPECT_EQ(0xFE, emu.sp);
    emu.StackPush(0xBC);
    EXPECT_EQ(0xFD, emu.sp);
    EXPECT_EQ(0xBC, emu.StackPop());
    EXPECT_EQ(0xFE, emu.sp);
    emu.StackPush(0xCD);
    EXPECT_EQ(0xFD, emu.sp);
    EXPECT_EQ(0xCD, emu.StackPop());
    EXPECT_EQ(0xFE, emu.sp);
    EXPECT_EQ(0xAB,emu.StackPop());
    EXPECT_EQ(0xFF, emu.sp);

	emu.Ins_ldx_imm(0x20);
    emu.Ins_txs_x_sp();
    EXPECT_EQ(0xFE,emu.sp);
    EXPECT_EQ(emu.mem[0x1FF],0x20);
}

TEST(emulator_meta, Flags) {
	uint16_t base_addr = 0;
    Emulator emu(base_addr);
    EXPECT_EQ(0b00100100, emu.sr);
    EXPECT_EQ(1, emu.TestFlag(FLAG_INTERRUPT));

    emu.SetFlag(1, FLAG_NEGATIVE);
    EXPECT_EQ(0b10100100, emu.sr);
    EXPECT_EQ(1, emu.TestFlag(FLAG_NEGATIVE));
    emu.SetFlag(1, FLAG_CARRY);
    EXPECT_EQ(0b10100101, emu.sr);
    EXPECT_EQ(1, emu.TestFlag(FLAG_CARRY));
    emu.SetFlag(0,FLAG_CARRY);
    EXPECT_EQ(0b10100100, emu.sr);
    EXPECT_EQ(0,emu.TestFlag(FLAG_CARRY));
}

TEST(instructions, Stack_Register) {
	uint16_t base_addr = 0;
    Emulator emu(base_addr);
    EXPECT_EQ(0b00100100, emu.sr);
    emu.SetFlag(1, FLAG_CARRY);
    emu.Ins_php();
	EXPECT_EQ(0b00100101, emu.ReadMem(0x01FF));

	emu.x = 0x40;
	emu.Ins_txs_x_sp();
	EXPECT_EQ(0x40, emu.ReadMem(0x01FE));

}


TEST(instructions, Jumps) {
	uint16_t base_addr = 0x20FF;
    Emulator emu(base_addr);
    EXPECT_EQ(0x20FF, emu.pc);
    emu.Ins_jsr(0x10FE);
    EXPECT_EQ(0x10FF, emu.pc);
    EXPECT_EQ(0x20, emu.ReadMem(0x1FF));
    EXPECT_EQ(0xFE, emu.ReadMem(0x1FE));
    
    emu.Ins_jmp_abs(0x15FF);
    EXPECT_EQ(0x15FF, emu.pc);

    emu.Ins_rts();
    EXPECT_EQ(0xFF, emu.sp);
    EXPECT_EQ(0x20FF, emu.pc);
}

TEST(instructions, Compares) {
	uint16_t base_addr = 0x0040;
    Emulator emu(base_addr);

    // Test Ins_cpy_imm();
    emu.y = 0x20;
    emu.Ins_cpy_imm(0x10);
    EXPECT_EQ(0, emu.TestFlag(FLAG_ZERO));
    EXPECT_EQ(0, emu.TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(0, emu.TestFlag(FLAG_CARRY));

    emu.Ins_cpy_imm(0x20);
    EXPECT_EQ(1, emu.TestFlag(FLAG_ZERO));
    EXPECT_EQ(0, emu.TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(0, emu.TestFlag(FLAG_CARRY));
    emu.SetFlag(0, FLAG_ZERO);

    emu.Ins_cpy_imm(0x30);
    EXPECT_EQ(0, emu.TestFlag(FLAG_ZERO));
    EXPECT_EQ(1, emu.TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(0, emu.TestFlag(FLAG_CARRY));
    emu.SetFlag(0, FLAG_NEGATIVE);

    emu.Ins_cpy_imm(0xFE);
    EXPECT_EQ(0, emu.TestFlag(FLAG_ZERO));
    EXPECT_EQ(0, emu.TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(1, emu.TestFlag(FLAG_CARRY));
    emu.SetFlag(0, FLAG_CARRY);
}

TEST(instructions, Branches) {
	uint16_t base_addr = 0x0040;
    Emulator emu(base_addr);

    // Test Ins_bpl() with 00XX and XXXX addresses. Second Byte should overflow;
    emu.Ins_bpl(0x20);
    EXPECT_EQ(0x60, emu.pc);
    emu.Ins_bpl(0xB0);
    EXPECT_EQ(0x10, emu.pc);

    emu.SetFlag(1, FLAG_NEGATIVE);
    emu.Ins_bpl(0x20);
    EXPECT_EQ(0x10, emu.pc);
    emu.SetFlag(0, FLAG_NEGATIVE);
 
    emu.pc = 0x2040;
    emu.Ins_bpl(0x20);
    EXPECT_EQ(0x2060, emu.pc);
    emu.Ins_bpl(0xB0);
    EXPECT_EQ(0x2010, emu.pc);

    emu.SetFlag(1, FLAG_NEGATIVE);
    emu.Ins_bpl(0x20);
    EXPECT_EQ(0x2010, emu.pc);
    emu.SetFlag(0, FLAG_NEGATIVE);

    // Test Ins_bne()
    emu.pc = 0x0040;
    emu.Ins_bne(0x20);
    EXPECT_EQ(0x60, emu.pc);
    emu.Ins_bne(0xB0);
    EXPECT_EQ(0x10, emu.pc);

    emu.SetFlag(1, FLAG_ZERO);
    emu.Ins_bne(0x20);
    EXPECT_EQ(0x10, emu.pc);
    emu.SetFlag(0, FLAG_ZERO);
 
    emu.pc = 0x2040;
    emu.Ins_bne(0x20);
    EXPECT_EQ(0x2060, emu.pc);
    emu.Ins_bne(0xB0);
    EXPECT_EQ(0x2010, emu.pc);

    emu.SetFlag(1, FLAG_ZERO);
    emu.Ins_bne(0x20);
    EXPECT_EQ(0x2010, emu.pc);
    emu.SetFlag(0, FLAG_ZERO);

    // Test Ins_beq()
    emu.pc = 0x0040;
    emu.SetFlag(1, FLAG_ZERO);
    emu.Ins_beq(0x20);
    EXPECT_EQ(0x60, emu.pc);
    emu.Ins_beq(0xB0);
    EXPECT_EQ(0x10, emu.pc);

    emu.SetFlag(0, FLAG_ZERO);
    emu.Ins_beq(0x20);
    EXPECT_EQ(0x10, emu.pc);
    emu.SetFlag(1, FLAG_ZERO);
 
    emu.pc = 0x2040;
    emu.Ins_beq(0x20);
    EXPECT_EQ(0x2060, emu.pc);
    emu.Ins_beq(0xB0);
    EXPECT_EQ(0x2010, emu.pc);

    emu.SetFlag(0, FLAG_ZERO);
    emu.Ins_beq(0x20);
    EXPECT_EQ(0x2010, emu.pc);
}

TEST(instructions, Loads) {
	uint16_t base_addr = 0x0000;
    Emulator emu(base_addr);
    emu.WriteMem(0x0020, 0x22);
    emu.WriteMem(0x0030, 0x33);
    emu.WriteMem(0x0040, 0x44);

    emu.WriteMem(0x2020, 0x23);
    emu.WriteMem(0x3030, 0x34);
    emu.WriteMem(0x4040, 0x45);

    emu.WriteMem(0x2040, 0x24);
    emu.WriteMem(0x3050, 0x35);
    emu.WriteMem(0x4060, 0x46);


    // Text ld-_imm
    emu.Ins_ldx_imm(0x20);
    EXPECT_EQ(0x20, emu.x);
    emu.Ins_ldy_imm(0x30);
    EXPECT_EQ(0x30, emu.y);
    emu.Ins_lda_imm(0x40);
    EXPECT_EQ(0x40, emu.ac);

    // Text ld-_abs
    emu.Ins_ldx_abs(0x2020);
    EXPECT_EQ(0x23, emu.x);
    emu.Ins_ldy_abs(0x3030);
    EXPECT_EQ(0x34, emu.y);
    emu.Ins_lda_abs(0x4040);
    EXPECT_EQ(0x45, emu.ac);

    // Text ld-_zer
    emu.Ins_ldx_zer(0x20);
    EXPECT_EQ(0x22, emu.x);
    emu.Ins_ldy_zer(0x30);
    EXPECT_EQ(0x33, emu.y);
    emu.Ins_lda_zer(0x40);
    EXPECT_EQ(0x44, emu.ac);

    // Test ldx_zeroy
    emu.Ins_ldy_imm(0x70);
    emu.Ins_ldx_zery(0xC0);
    EXPECT_EQ(0x33, emu.x);

    emu.Ins_ldy_imm(0x10);
    emu.Ins_ldx_zery(0x10);
    EXPECT_EQ(0x22, emu.x);
    emu.Ins_ldx_zery(0x30);
    EXPECT_EQ(0x44, emu.x);

    // Test ldy_zero_x
    emu.Ins_ldx_imm(0x70);
    emu.Ins_ldy_zerx(0xC0);
    EXPECT_EQ(0x33, emu.y);

    emu.Ins_ldx_imm(0x10);
    emu.Ins_ldy_zerx(0x10);
    EXPECT_EQ(0x22, emu.y);
    emu.Ins_ldy_zerx(0x30);
    EXPECT_EQ(0x44, emu.y);

    // Test lda_zerox
    emu.Ins_ldx_imm(0x70);
    emu.Ins_lda_zerx(0xC0);
    EXPECT_EQ(0x33, emu.ac);

    emu.Ins_ldx_imm(0x10);
    emu.Ins_lda_zerx(0x10);
    EXPECT_EQ(0x22, emu.ac);
    emu.Ins_lda_zerx(0x30);
    EXPECT_EQ(0x44, emu.ac);

    // Test ldx_absy
    emu.Ins_ldy_imm(0x40);
    emu.Ins_ldx_absy(0x2000);
    EXPECT_EQ(0x24, emu.x);
    emu.Ins_ldy_imm(0x20);
    emu.Ins_ldx_absy(0x2000);
    EXPECT_EQ(0x23, emu.x);

    // Test ldy_absx
    emu.Ins_ldx_imm(0x40);
    emu.Ins_ldy_absx(0x2000);
    EXPECT_EQ(0x24, emu.y);
    emu.Ins_ldx_imm(0x20);
    emu.Ins_ldy_absx(0x2000);
    EXPECT_EQ(0x23, emu.y);

    // Test lda_absy
    emu.Ins_ldy_imm(0x40);
    emu.Ins_lda_absy(0x2000);
    EXPECT_EQ(0x24, emu.ac);
    emu.Ins_ldy_imm(0x20);
    emu.Ins_lda_absy(0x2000);
    EXPECT_EQ(0x23, emu.ac);

    // Test lda_absx
    emu.Ins_ldx_imm(0x40);
    emu.Ins_lda_absx(0x2000);
    EXPECT_EQ(0x24, emu.ac);
    emu.Ins_ldx_imm(0x20);
    emu.Ins_lda_absy(0x2000);
    EXPECT_EQ(0x23, emu.ac);

}

TEST(instructions, Logical) {
	uint16_t base_addr = 0;
    Emulator emu(base_addr);

    // Test asl_acc() and that it sets the right flags
    emu.Ins_lda_imm(0x90);
    emu.Ins_asl_acc();
    EXPECT_EQ(1, emu.TestFlag(FLAG_CARRY));
    EXPECT_EQ(0b00100000, emu.ac);
    emu.SetFlag(0, emu.TestFlag(FLAG_CARRY));

    emu.Ins_lda_imm(0x80);
    emu.Ins_asl_acc();
    EXPECT_EQ(1, emu.TestFlag(FLAG_CARRY));
    EXPECT_EQ(1, emu.TestFlag(FLAG_ZERO));
    EXPECT_EQ(0b00000000, emu.ac);
    emu.SetFlag(0, emu.TestFlag(FLAG_CARRY));
    emu.SetFlag(0, emu.TestFlag(FLAG_ZERO));

    emu.Ins_lda_imm(0x40);
    emu.Ins_asl_acc();
    EXPECT_EQ(1, emu.TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(0b10000000, emu.ac);
    emu.SetFlag(0, emu.TestFlag(FLAG_NEGATIVE));    

    emu.Ins_lda_imm(0x00);
    emu.Ins_asl_acc();
    EXPECT_EQ(1, emu.TestFlag(FLAG_ZERO));
    EXPECT_EQ(0b00000000, emu.ac);
    emu.SetFlag(0, emu.TestFlag(FLAG_ZERO));    

    // Test asl_acc_zp() and that it sets the right flags
    emu.WriteMem(0x0010,0x90);
    emu.Ins_asl_zp(0x10);
    EXPECT_EQ(1, emu.TestFlag(FLAG_CARRY));
    EXPECT_EQ(0b00100000, emu.ReadMem(0x10));
    emu.SetFlag(0, emu.TestFlag(FLAG_CARRY));

    emu.WriteMem(0x0010,0x80);
    emu.Ins_asl_zp(0x10);
    EXPECT_EQ(1, emu.TestFlag(FLAG_CARRY));
    EXPECT_EQ(1, emu.TestFlag(FLAG_ZERO));
    EXPECT_EQ(0b00000000, emu.ReadMem(0x10));
    emu.SetFlag(0, emu.TestFlag(FLAG_CARRY));
    emu.SetFlag(0, emu.TestFlag(FLAG_ZERO));

    emu.WriteMem(0x0010,0x40);
    emu.Ins_asl_zp(0x10);
    EXPECT_EQ(1, emu.TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(0b10000000, emu.ReadMem(0x10));
    emu.SetFlag(0, emu.TestFlag(FLAG_NEGATIVE));    

    emu.WriteMem(0x0010,0x00);
    emu.Ins_asl_zp(0x10);
    EXPECT_EQ(1, emu.TestFlag(FLAG_ZERO));
    EXPECT_EQ(0b00000000, emu.ReadMem(0x10));
    emu.SetFlag(0, emu.TestFlag(FLAG_ZERO));   
}