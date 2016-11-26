#include "6502.h"
#include "gtest/gtest.h"


// Test creating 6502 object
TEST(emulator_object_test, Suceess) {
  // This test is named "Negative", and belongs to the "FactorialTest"
  // test case.
	uint16_t base_addr = 20;
    Emulator emu(base_addr);
    EXPECT_EQ(20, emu.pc);
}
// Test that value is set correctly