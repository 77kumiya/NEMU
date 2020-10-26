#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/

#define dest_prep               \
    opr_dest.type = OPR_MEM;    \
    opr_dest.addr = cpu.esp;    \
    opr_dest.sreg = SREG_SS;    \
    opr_dest.val = opr_src.val;

static void instr_execute_1op(){
    operand_read(&opr_src);
    cpu.esp -= opr_src.data_size / 8;
    dest_prep
    operand_write(&opr_dest);
}

make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)
make_instr_impl_1op(push, i, b)
make_instr_impl_1op(push, i, v)