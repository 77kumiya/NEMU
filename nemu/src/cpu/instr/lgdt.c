#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/

static void instr_execute_1op(){
    cpu.gdtr.limit = paddr_read(opr_src.addr, 2);
    cpu.gdtr.base = paddr_read(opr_src.addr + 2, 4);
}

make_instr_impl_1op(lgdt, m, v)
