#include "cpu/instr.h"

static void instr_execute_2op(){
    operand_read(&opr_src);
    operand_read(&opr_dest);
    alu_sub(opr_src.val, dest_src.val, dest_src.data_size);
}

/*
Put the implementations of `cmp' instructions here.
*/
make_instr_impl_2op(cmp, i, rm, bv)
