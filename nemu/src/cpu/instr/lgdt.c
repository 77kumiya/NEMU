#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/

static void instr_execute_1op(){
    opr_src.data_size = 16;
    operand_read(&opr_src);
    cpu.ldgt.limit = opr_src.val;
    
    opr_src.data_size = 32;
    opr_src.addr += 2;
    operand_read(&opr_src);
    cpu.ldgt.base = opr_src.val;
}

make_instr_impl_1op(lgdt, rm, v)