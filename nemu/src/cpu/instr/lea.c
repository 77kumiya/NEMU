#include "cpu/instr.h"
/*
Put the implementations of `lea' instructions here.
*/

static void instr_execute_2op()
{
        opr_dest.val = opr_src.addr;
        operand_write(&opr_dest);
}

make_instr_func(lea)
{
        int len = 1;
		opr_dest.data_size = data_size;
		decode_operand_rm2r
		
		print_asm_2("lea", "", len, &opr_src, &opr_dest);
		
		instr_execute_2op();
		return len; 
}
