#include "gtest/gtest.h"
#include "6502.h"
//#include "Misc.h"
// Test creating 6502 object
TEST(emulator_object_test, Suceess) {
  // This test is named "Negative", and belongs to the "FactorialTest"
  // test case.
    Emulator::getInstance().SetBaseAddr(20);
    EXPECT_EQ(20, Emulator::getInstance().pc);
}
// Test that value is set correctly


TEST(emulator_meta, Memory) {
  // This test is named "Negative", and belongs to the "FactorialTest"
  // test case.

    Emulator::getInstance().SetBaseAddr(0x0000);
    Emulator::getInstance().WriteMem(0x1000, 0x10);
    EXPECT_EQ(0xFF, Emulator::getInstance().ReadMem(0x0000));
    EXPECT_EQ(0x10, Emulator::getInstance().ReadMem(0x1000));
}

TEST(emulator_meta, Stack) {

    Emulator::getInstance().SetBaseAddr(0x0000);

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
    EXPECT_EQ(0xFF, Emulator::getInstance().sp);
    Emulator::getInstance().StackPush(0xAB);
    EXPECT_EQ(0xFE, Emulator::getInstance().sp);
    Emulator::getInstance().StackPush(0xBC);
    EXPECT_EQ(0xFD, Emulator::getInstance().sp);
    EXPECT_EQ(0xBC, Emulator::getInstance().StackPop());
    EXPECT_EQ(0xFE, Emulator::getInstance().sp);
    Emulator::getInstance().StackPush(0xCD);
    EXPECT_EQ(0xFD, Emulator::getInstance().sp);
    EXPECT_EQ(0xCD, Emulator::getInstance().StackPop());
    EXPECT_EQ(0xFE, Emulator::getInstance().sp);
    EXPECT_EQ(0xAB,Emulator::getInstance().StackPop());
    EXPECT_EQ(0xFF, Emulator::getInstance().sp);

        Emulator::getInstance().x = 0x20;
    Emulator::getInstance().Ins_txs_x_sp();
    EXPECT_EQ(0xFE,Emulator::getInstance().sp);
    EXPECT_EQ(Emulator::getInstance().mem[0x1FF],0x20);
}

TEST(emulator_meta, Flags) {

    Emulator::getInstance().SetBaseAddr(0x0000);
    EXPECT_EQ(0b00100100, Emulator::getInstance().sr);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_INTERRUPT));

    Emulator::getInstance().SetFlag(1, FLAG_NEGATIVE);
    EXPECT_EQ(0b10100100, Emulator::getInstance().sr);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
    Emulator::getInstance().SetFlag(1, FLAG_CARRY);
    EXPECT_EQ(0b10100101, Emulator::getInstance().sr);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_CARRY));
    Emulator::getInstance().SetFlag(0,FLAG_CARRY);
    EXPECT_EQ(0b10100100, Emulator::getInstance().sr);
    EXPECT_EQ(0,Emulator::getInstance().TestFlag(FLAG_CARRY));
}

TEST(instructions, Stack_Register) {

    Emulator::getInstance().SetBaseAddr(0x0000);
    EXPECT_EQ(0b00100100, Emulator::getInstance().sr);
    Emulator::getInstance().SetFlag(1, FLAG_CARRY);
    Emulator::getInstance().Ins_php();
        EXPECT_EQ(0b00100101, Emulator::getInstance().ReadMem(0x01FF));

        Emulator::getInstance().x = 0x40;
        Emulator::getInstance().Ins_txs_x_sp();
        EXPECT_EQ(0x40, Emulator::getInstance().ReadMem(0x01FE));

        Emulator::getInstance().SetFlag(1, FLAG_DECIMAL);
        Emulator::getInstance().Ins_cld();
        EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_DECIMAL));

        Emulator::getInstance().Ins_cld();
        EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_DECIMAL));
}


TEST(instructions, Jumps) {

    Emulator::getInstance().SetBaseAddr(0x20FF);
    EXPECT_EQ(0x20FF, Emulator::getInstance().pc);
    Emulator::getInstance().pc++;
    Emulator::getInstance().pc++;
    Emulator::getInstance().Ins_jsr(0x10FF);
    EXPECT_EQ(0x10FF, Emulator::getInstance().pc);
    EXPECT_EQ(0x21, Emulator::getInstance().ReadMem(0x1FF)); // Verify Stack
    EXPECT_EQ(0x00, Emulator::getInstance().ReadMem(0x1FE)); //	Verift Stack
    
    Emulator::getInstance().Ins_jmp_abs(0x15FF);
    EXPECT_EQ(0x15FF, Emulator::getInstance().pc);

    Emulator::getInstance().Ins_rts();
    EXPECT_EQ(0xFF, Emulator::getInstance().sp);
    EXPECT_EQ(0x2101, Emulator::getInstance().pc);
}

TEST(instructions, Math) {

    Emulator::getInstance().SetBaseAddr(0x0040);

    // Test Ins_inc_y
    Emulator::getInstance().y = 0x20;
    Emulator::getInstance().Ins_inc_y();
    EXPECT_EQ(0x21, Emulator::getInstance().y);

    Emulator::getInstance().y = 0xFF;
    Emulator::getInstance().Ins_inc_y();
    EXPECT_EQ(0x00, Emulator::getInstance().y);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));

    Emulator::getInstance().y = 0x7F;
    Emulator::getInstance().Ins_inc_y();
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));


    // Test Ins_dex
    Emulator::getInstance().x = 0x20;
    Emulator::getInstance().Ins_dex();
    EXPECT_EQ(0x1F, Emulator::getInstance().x);
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
 
    Emulator::getInstance().x = 0x01;
    Emulator::getInstance().Ins_dex();
    EXPECT_EQ(0x00, Emulator::getInstance().x);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));

    Emulator::getInstance().x = 0x90;
    Emulator::getInstance().Ins_dex();
    EXPECT_EQ(0x8F, Emulator::getInstance().x);
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));

    // Test Ins_dey
    Emulator::getInstance().y = 0x20;
    Emulator::getInstance().Ins_dey();
    EXPECT_EQ(0x1F, Emulator::getInstance().y);
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));

    Emulator::getInstance().y = 0x01;
    Emulator::getInstance().Ins_dey();
    EXPECT_EQ(0x00, Emulator::getInstance().y);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));

    Emulator::getInstance().y = 0x90;
    Emulator::getInstance().Ins_dey();
    EXPECT_EQ(0x8F, Emulator::getInstance().y);
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
    
}

TEST(instructions, Compares) {

    Emulator::getInstance().SetBaseAddr(0x0040);

    // Test Ins_cpy_imm();
    Emulator::getInstance().y = 0x20;
    Emulator::getInstance().Ins_cpy_imm(0x10);
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_CARRY));

    Emulator::getInstance().Ins_cpy_imm(0x20);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_CARRY));
    Emulator::getInstance().SetFlag(0, FLAG_ZERO);

    Emulator::getInstance().Ins_cpy_imm(0x30);
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_CARRY));
    Emulator::getInstance().SetFlag(0, FLAG_NEGATIVE);

    Emulator::getInstance().Ins_cpy_imm(0xFE);
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_CARRY));
    Emulator::getInstance().SetFlag(0, FLAG_CARRY);
}

TEST(instructions, Branches) {

    Emulator::getInstance().SetBaseAddr(0x0040);

    // Test Ins_bpl() with 00XX and XXXX addresses. Second Byte should overflow;
    Emulator::getInstance().Ins_bpl(0x20);
    EXPECT_EQ(0x60, Emulator::getInstance().pc);
    Emulator::getInstance().Ins_bpl(0xB0);
    EXPECT_EQ(0x10, Emulator::getInstance().pc);

    Emulator::getInstance().SetFlag(1, FLAG_NEGATIVE);
    Emulator::getInstance().Ins_bpl(0x20);
    EXPECT_EQ(0x10, Emulator::getInstance().pc);
    Emulator::getInstance().SetFlag(0, FLAG_NEGATIVE);
 
    Emulator::getInstance().pc = 0x2040;
    Emulator::getInstance().Ins_bpl(0x20);
    EXPECT_EQ(0x2060, Emulator::getInstance().pc);
    Emulator::getInstance().Ins_bpl(0xB0);
    EXPECT_EQ(0x2010, Emulator::getInstance().pc);

    Emulator::getInstance().SetFlag(1, FLAG_NEGATIVE);
    Emulator::getInstance().Ins_bpl(0x20);
    EXPECT_EQ(0x2010, Emulator::getInstance().pc);
    Emulator::getInstance().SetFlag(0, FLAG_NEGATIVE);

    // Test Ins_bne()
    Emulator::getInstance().pc = 0x0040;
    Emulator::getInstance().Ins_bne(0x20);
    EXPECT_EQ(0x60, Emulator::getInstance().pc);
    Emulator::getInstance().Ins_bne(0xB0);
    EXPECT_EQ(0x10, Emulator::getInstance().pc);

    Emulator::getInstance().SetFlag(1, FLAG_ZERO);
    Emulator::getInstance().Ins_bne(0x20);
    EXPECT_EQ(0x10, Emulator::getInstance().pc);
    Emulator::getInstance().SetFlag(0, FLAG_ZERO);
 
    Emulator::getInstance().pc = 0x2040;
    Emulator::getInstance().Ins_bne(0x20);
    EXPECT_EQ(0x2060, Emulator::getInstance().pc);
    Emulator::getInstance().Ins_bne(0xB0);
    EXPECT_EQ(0x2010, Emulator::getInstance().pc);

    Emulator::getInstance().SetFlag(1, FLAG_ZERO);
    Emulator::getInstance().Ins_bne(0x20);
    EXPECT_EQ(0x2010, Emulator::getInstance().pc);
    Emulator::getInstance().SetFlag(0, FLAG_ZERO);

    // Test Ins_beq()
    Emulator::getInstance().pc = 0x0040;
    Emulator::getInstance().SetFlag(1, FLAG_ZERO);
    Emulator::getInstance().Ins_beq(0x20);
    EXPECT_EQ(0x60, Emulator::getInstance().pc);
    Emulator::getInstance().Ins_beq(0xB0);
    EXPECT_EQ(0x10, Emulator::getInstance().pc);

    Emulator::getInstance().SetFlag(0, FLAG_ZERO);
    Emulator::getInstance().Ins_beq(0x20);
    EXPECT_EQ(0x10, Emulator::getInstance().pc);
    Emulator::getInstance().SetFlag(1, FLAG_ZERO);
 
    Emulator::getInstance().pc = 0x2040;
    Emulator::getInstance().Ins_beq(0x20);
    EXPECT_EQ(0x2060, Emulator::getInstance().pc);
    Emulator::getInstance().Ins_beq(0xB0);
    EXPECT_EQ(0x2010, Emulator::getInstance().pc);

    Emulator::getInstance().SetFlag(0, FLAG_ZERO);
    Emulator::getInstance().Ins_beq(0x20);
    EXPECT_EQ(0x2010, Emulator::getInstance().pc);
}

TEST(instructions, Loads) {

    Emulator::getInstance().SetBaseAddr(0x0000);
    Emulator::getInstance().WriteMem(0x0020, 0x22);
    Emulator::getInstance().WriteMem(0x0030, 0x33);
    Emulator::getInstance().WriteMem(0x0040, 0x44);

    Emulator::getInstance().WriteMem(0x2020, 0x23);
    Emulator::getInstance().WriteMem(0x3030, 0x34);
    Emulator::getInstance().WriteMem(0x4040, 0x45);

    Emulator::getInstance().WriteMem(0x2040, 0x24);
    Emulator::getInstance().WriteMem(0x3050, 0x35);
    Emulator::getInstance().WriteMem(0x4060, 0x46);


    // Text ld-_imm
    Emulator::getInstance().Ins_ldx_imm(0x20);
    EXPECT_EQ(0x20, Emulator::getInstance().x);
    Emulator::getInstance().Ins_ldy_imm(0x30);
    EXPECT_EQ(0x30, Emulator::getInstance().y);
    Emulator::getInstance().Ins_lda_imm(0x40);
    EXPECT_EQ(0x40, Emulator::getInstance().ac);

    // Text ld-_abs
    Emulator::getInstance().Ins_ldx_abs(0x2020);
    EXPECT_EQ(0x23, Emulator::getInstance().x);
    Emulator::getInstance().Ins_ldy_abs(0x3030);
    EXPECT_EQ(0x34, Emulator::getInstance().y);
    Emulator::getInstance().Ins_lda_abs(0x4040);
    EXPECT_EQ(0x45, Emulator::getInstance().ac);

    // Text ld-_zer
    Emulator::getInstance().Ins_ldx_zer(0x20);
    EXPECT_EQ(0x22, Emulator::getInstance().x);
    Emulator::getInstance().Ins_ldy_zer(0x30);
    EXPECT_EQ(0x33, Emulator::getInstance().y);
    Emulator::getInstance().Ins_lda_zer(0x40);
    EXPECT_EQ(0x44, Emulator::getInstance().ac);

    // Test ldx_zeroy
    Emulator::getInstance().Ins_ldy_imm(0x70);
    Emulator::getInstance().Ins_ldx_zery(0xC0);
    EXPECT_EQ(0x33, Emulator::getInstance().x);

    Emulator::getInstance().Ins_ldy_imm(0x10);
    Emulator::getInstance().Ins_ldx_zery(0x10);
    EXPECT_EQ(0x22, Emulator::getInstance().x);
    Emulator::getInstance().Ins_ldx_zery(0x30);
    EXPECT_EQ(0x44, Emulator::getInstance().x);

    // Test ldy_zero_x
    Emulator::getInstance().Ins_ldx_imm(0x70);
    Emulator::getInstance().Ins_ldy_zerx(0xC0);
    EXPECT_EQ(0x33, Emulator::getInstance().y);

    Emulator::getInstance().Ins_ldx_imm(0x10);
    Emulator::getInstance().Ins_ldy_zerx(0x10);
    EXPECT_EQ(0x22, Emulator::getInstance().y);
    Emulator::getInstance().Ins_ldy_zerx(0x30);
    EXPECT_EQ(0x44, Emulator::getInstance().y);

    // Test lda_zerox
    Emulator::getInstance().Ins_ldx_imm(0x70);
    Emulator::getInstance().Ins_lda_zerx(0xC0);
    EXPECT_EQ(0x33, Emulator::getInstance().ac);

    Emulator::getInstance().Ins_ldx_imm(0x10);
    Emulator::getInstance().Ins_lda_zerx(0x10);
    EXPECT_EQ(0x22, Emulator::getInstance().ac);
    Emulator::getInstance().Ins_lda_zerx(0x30);
    EXPECT_EQ(0x44, Emulator::getInstance().ac);

    // Test ldx_absy
    Emulator::getInstance().Ins_ldy_imm(0x40);
    Emulator::getInstance().Ins_ldx_absy(0x2000);
    EXPECT_EQ(0x24, Emulator::getInstance().x);
    Emulator::getInstance().Ins_ldy_imm(0x20);
    Emulator::getInstance().Ins_ldx_absy(0x2000);
    EXPECT_EQ(0x23, Emulator::getInstance().x);

    // Test ldy_absx
    Emulator::getInstance().Ins_ldx_imm(0x40);
    Emulator::getInstance().Ins_ldy_absx(0x2000);
    EXPECT_EQ(0x24, Emulator::getInstance().y);
    Emulator::getInstance().Ins_ldx_imm(0x20);
    Emulator::getInstance().Ins_ldy_absx(0x2000);
    EXPECT_EQ(0x23, Emulator::getInstance().y);

    // Test lda_absy
    Emulator::getInstance().Ins_ldy_imm(0x40);
    Emulator::getInstance().Ins_lda_absy(0x2000);
    EXPECT_EQ(0x24, Emulator::getInstance().ac);
    Emulator::getInstance().Ins_ldy_imm(0x20);
    Emulator::getInstance().Ins_lda_absy(0x2000);
    EXPECT_EQ(0x23, Emulator::getInstance().ac);

    // Test lda_absx
    Emulator::getInstance().Ins_ldx_imm(0x40);
    Emulator::getInstance().Ins_lda_absx(0x2000);
    EXPECT_EQ(0x24, Emulator::getInstance().ac);
    Emulator::getInstance().Ins_ldx_imm(0x20);
    Emulator::getInstance().Ins_lda_absy(0x2000);
    EXPECT_EQ(0x23, Emulator::getInstance().ac);

}

TEST(instructions, Logical) {

    Emulator::getInstance().SetBaseAddr(0x0000);

    // Test Ins_ora_acc
    Emulator::getInstance().ac = 0b01010101;
    Emulator::getInstance().x =  0x10;
    Emulator::getInstance().WriteMem(0x30, 0x10);
    Emulator::getInstance().WriteMem(0x31, 0x30);
    Emulator::getInstance().WriteMem(0x3010, 0b10100000);
    Emulator::getInstance().Ins_ora_ind_x(0x20);
    EXPECT_EQ(0b11110101, Emulator::getInstance().ac);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
        EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_ZERO));

    Emulator::getInstance().ac = 0b00000000;
    Emulator::getInstance().x =  0x10;
    Emulator::getInstance().WriteMem(0x30, 0x10);
    Emulator::getInstance().WriteMem(0x31, 0x30);
    Emulator::getInstance().WriteMem(0x3010, 0b00000000);
    Emulator::getInstance().Ins_ora_ind_x(0x20);
    EXPECT_EQ(0b00000000, Emulator::getInstance().ac);
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));

    // Test Ins_eor_imm
    Emulator::getInstance().ac = 0b10101010;
    Emulator::getInstance().Ins_eor_imm(0b01010101);
    EXPECT_EQ(0b11111111, Emulator::getInstance().ac);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_ZERO));

    Emulator::getInstance().ac = 0b11111111;
    Emulator::getInstance().Ins_eor_imm(0b10101010);
    EXPECT_EQ(0b01010101, Emulator::getInstance().ac);
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
        EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_ZERO));

    Emulator::getInstance().ac = 0b11111111;
    Emulator::getInstance().Ins_eor_imm(0b11111111);
    EXPECT_EQ(0b00000000, Emulator::getInstance().ac);
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
        EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));

    // Test asl_acc() and that it sets the right flags
    Emulator::getInstance().Ins_lda_imm(0x90);
    Emulator::getInstance().Ins_asl_acc();
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_CARRY));
    EXPECT_EQ(0b00100000, Emulator::getInstance().ac);
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_CARRY));

    Emulator::getInstance().Ins_lda_imm(0x80);
    Emulator::getInstance().Ins_asl_acc();
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_CARRY));
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0b00000000, Emulator::getInstance().ac);
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_CARRY));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_ZERO));

    Emulator::getInstance().Ins_lda_imm(0x40);
    Emulator::getInstance().Ins_asl_acc();
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(0b10000000, Emulator::getInstance().ac);
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));

    Emulator::getInstance().Ins_lda_imm(0x00);
    Emulator::getInstance().Ins_asl_acc();
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0b00000000, Emulator::getInstance().ac);
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_ZERO));

    // Test asl_acc_zp() and that it sets the right flags
    Emulator::getInstance().WriteMem(0x0010,0x90);
    Emulator::getInstance().Ins_asl_zp(0x10);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_CARRY));
    EXPECT_EQ(0b00100000, Emulator::getInstance().ReadMem(0x10));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_CARRY));

    Emulator::getInstance().WriteMem(0x0010,0x80);
    Emulator::getInstance().Ins_asl_zp(0x10);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_CARRY));
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0b00000000, Emulator::getInstance().ReadMem(0x10));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_CARRY));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_ZERO));

    Emulator::getInstance().WriteMem(0x0010,0x40);
    Emulator::getInstance().Ins_asl_zp(0x10);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(0b10000000, Emulator::getInstance().ReadMem(0x10));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));

    Emulator::getInstance().WriteMem(0x0010,0x00);
    Emulator::getInstance().Ins_asl_zp(0x10);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0b00000000, Emulator::getInstance().ReadMem(0x10));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_ZERO));

    // Test asl_abs_x() and that it sets the right flags
    Emulator::getInstance().WriteMem(0x1010,0x90);
    Emulator::getInstance().x = 0x10;
    Emulator::getInstance().Ins_asl_abs_x(0x1000);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_CARRY));
    EXPECT_EQ(0b00100000, Emulator::getInstance().ReadMem(0x1010));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_CARRY));

    Emulator::getInstance().WriteMem(0x1010,0x80);
    Emulator::getInstance().Ins_asl_abs_x(0x1000);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_CARRY));
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0b00000000, Emulator::getInstance().ReadMem(0x1010));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_CARRY));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_ZERO));

    Emulator::getInstance().WriteMem(0x1010,0x40);
    Emulator::getInstance().Ins_asl_abs_x(0x1000);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(0b10000000, Emulator::getInstance().ReadMem(0x1010));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));

    Emulator::getInstance().WriteMem(0x1010,0x00);
    Emulator::getInstance().Ins_asl_abs_x(0x1000);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0b00000000, Emulator::getInstance().ReadMem(0x1010));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_ZERO));

    // Test asl_abs() and that it sets the right flags
    Emulator::getInstance().WriteMem(0x1010,0x90);
    Emulator::getInstance().Ins_asl_abs(0x1010);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_CARRY));
    EXPECT_EQ(0b00100000, Emulator::getInstance().ReadMem(0x1010));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_CARRY));

    Emulator::getInstance().WriteMem(0x1010,0x80);
    Emulator::getInstance().Ins_asl_abs(0x1010);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_CARRY));
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0b00000000, Emulator::getInstance().ReadMem(0x1010));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_CARRY));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_ZERO));

    Emulator::getInstance().WriteMem(0x1010,0x40);
    Emulator::getInstance().Ins_asl_abs(0x1010);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));
    EXPECT_EQ(0b10000000, Emulator::getInstance().ReadMem(0x1010));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));

    Emulator::getInstance().WriteMem(0x1010,0x00);
    Emulator::getInstance().Ins_asl_abs(0x1010);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0b00000000, Emulator::getInstance().ReadMem(0x1010));
    Emulator::getInstance().SetFlag(0, Emulator::getInstance().TestFlag(FLAG_ZERO));
}

TEST(instructions, Stores) {

    Emulator::getInstance().SetBaseAddr(0x0000);

    Emulator::getInstance().x = 0x22;
    Emulator::getInstance().ac = 0x23;
    Emulator::getInstance().Ins_stx_zp(0x30);
    EXPECT_EQ(0x22, Emulator::getInstance().ReadMem(0x30));

    Emulator::getInstance().Ins_stx_abs(0x4030);
    EXPECT_EQ(0x22, Emulator::getInstance().ReadMem(0x4030));

    Emulator::getInstance().Ins_sta_abs(0x4030);
    EXPECT_EQ(0x23, Emulator::getInstance().ReadMem(0x4030));
}

TEST(instructions, Transfers){

    Emulator::getInstance().SetBaseAddr(0x0000);

    Emulator::getInstance().ac = 0x20;
    Emulator::getInstance().y = 0x10;
    Emulator::getInstance().Ins_tay();
    EXPECT_EQ(0x20, Emulator::getInstance().y);
    EXPECT_EQ(0x20, Emulator::getInstance().ac);
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));

    Emulator::getInstance().ac = 0x00;
    Emulator::getInstance().Ins_tay();
    EXPECT_EQ(0x00, Emulator::getInstance().y);
    EXPECT_EQ(0x00, Emulator::getInstance().ac);
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));

    Emulator::getInstance().ac = 0x81;
    Emulator::getInstance().Ins_tay();
    EXPECT_EQ(0x81, Emulator::getInstance().y);
    EXPECT_EQ(0x81, Emulator::getInstance().ac);
    EXPECT_EQ(0, Emulator::getInstance().TestFlag(FLAG_ZERO));
    EXPECT_EQ(1, Emulator::getInstance().TestFlag(FLAG_NEGATIVE));


}
