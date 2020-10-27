#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/

#define dest_prep               \
    opr_dest.type = OPR_MEM;    \
    opr_dest.addr = cpu.esp;    \
    opr_dest.sreg = SREG_SS;    \
    opr_dest.data_size = data_size; \
    opr_dest.val = sign_ext(opr_src.val, opr_src.data_size);

static void instr_execute_1op(){
    operand_read(&opr_src);
    cpu.esp -= data_size >> 3;
    dest_prep
    operand_write(&opr_dest);
}

make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)
make_instr_impl_1op(push, i, b)
make_instr_impl_1op(push, i, v)