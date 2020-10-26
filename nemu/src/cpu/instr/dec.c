#include "cpu/instr.h"
/*
Put the implementations of `dec' instructions here.
*/

static void instr_execute_1op()
{
    operand_read(&opr_src);
    bool original_CF = cpu.eflags.CF;  //dec does not change the carry flag, so keep the original CF
    opr_src.val = alu_sub(1, opr_src.val, opr_src.data_size);
    operand_write(&opr_src);
    cpu.eflags.CF = original_CF;
}

make_instr_impl_1op(dec, rm, v)
make_instr_impl_1op(dec, rm, b)
make_instr_impl_1op(dec, r, v)
