#include "cpu/instr.h"
/*
Put the implementations of `inc' instructions here.
*/

static void instr_execute_1op()
{
    operand_read(&opr_src);
    bool original_CF = cpu.eflags.CF;  //inc does not change the carry flag, so keep the original CF
    opr_src.val = alu_add(1, opr_src.val, opr_src.data_size);
    operand_write(&opr_src);
    cpu.eflags.CF = original_CF;
}

make_instr_impl_1op(inc, rm, v)
make_instr_impl_1op(inc, rm, b)
