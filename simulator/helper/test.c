// gcc -E test.c > file 
// cat file |grep ExecuteInst

namespace mos6502
{

#define DEFINST(name, op1, imm, reg, offs) \
		 Context::ExecuteInst_##name()\
{ throw misc::Panic("Unimplemented instruction"); }
#include "../src/Instruction.def"
#undef DEFINST
};


namespace mos6502
{

#define DEFINST(name, op1, imm, reg, offs) \
		 ExecuteInst_##name(#op1, #imm, #reg, #offs); // #op1, #imm, #reg, #offs
#include "../src/Instruction.def"
#undef DEFINST
};
