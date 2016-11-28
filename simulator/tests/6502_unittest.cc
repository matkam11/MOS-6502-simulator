#include "6502.h"
#include "gtest/gtest.h"
#include "Error.h"
#include "Misc.h"
// Test creating 6502 object
TEST(emulator_object_test, Suceess) {
  // This test is named "Negative", and belongs to the "FactorialTest"
  // test case.
	uint16_t base_addr = 20;
    Emulator emu(base_addr);
    EXPECT_EQ(20, emu.pc);
}
// Test that value is set correctly


TEST(emulator_memory_test, Suceess) {
  // This test is named "Negative", and belongs to the "FactorialTest"
  // test case.
	uint16_t base_addr = 0;
    Emulator emu(base_addr);
    emu.WriteMem(0x1000, 0x10);
    EXPECT_EQ(0xFF, emu.ReadMem(0x0000));
    EXPECT_EQ(0x10, emu.ReadMem(0x1000));

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
    EXPECT_EQ(0xFD,emu.sp);
    EXPECT_EQ(emu.mem[0x1FF],0x20);
}

TEST(instructions, Jumps) {
	uint16_t base_addr = 0x20FF;
    Emulator emu(base_addr);
    EXPECT_EQ(0x20FF, emu.pc);
    emu.Ins_jsr(0x10FF);
    EXPECT_EQ(0x10FF, emu.pc);
    EXPECT_EQ(0x20, emu.ReadMem(0x1FF));
    EXPECT_EQ(0xFE, emu.ReadMem(0x1FE));
    EXPECT_EQ(0x10FF, emu.pc);

    emu.Ins_jmp(0x15FF);
    EXPECT_EQ(0x15FF, emu.pc);

    emu.Ins_rts();
    EXPECT_EQ(0xFF, emu.sp);
    EXPECT_EQ(0x20FF, emu.pc);
}

TEST(instructions, loads) {
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
    emu.Ins_ldx_absy(0x20);
    EXPECT_EQ(0x24, emu.x);
    emu.Ins_ldy_imm(0x20);
    emu.Ins_ldx_absy(0x20);
    EXPECT_EQ(0x23, emu.x);

    // Test ldy_absx
    emu.Ins_ldx_imm(0x40);
    emu.Ins_ldy_absx(0x20);
    EXPECT_EQ(0x24, emu.y);
    emu.Ins_ldx_imm(0x20);
    emu.Ins_ldy_absx(0x20);
    EXPECT_EQ(0x23, emu.y);

    // Test lda_absy
    emu.Ins_ldy_imm(0x40);
    emu.Ins_lda_absy(0x20);
    EXPECT_EQ(0x24, emu.ac);
    emu.Ins_ldy_imm(0x20);
    emu.Ins_lda_absy(0x20);
    EXPECT_EQ(0x23, emu.ac);

    // Test lda_absx
    emu.Ins_ldx_imm(0x40);
    emu.Ins_lda_absx(0x20);
    EXPECT_EQ(0x24, emu.ac);
    emu.Ins_ldx_imm(0x20);
    emu.Ins_lda_absx(0x20);
    EXPECT_EQ(0x23, emu.ac);

}