#include "cpu/instr.h"
/*
Put the implementations of `lidt' instructions here.
*/

static void instr_execute_1op(){
    opr_src.data_size = 16;
    operand_read(&opr_src);
    cpu.idtr.limit = opr_src.val;
    
    opr_src.data_size = 32;
    opr_src.addr += 2;
    operand_read(&opr_src);
    cpu.idtr.base = opr_src.val;
}

make_instr_impl_1op(lidt, rm, v)
