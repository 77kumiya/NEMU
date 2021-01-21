#include "cpu/intr.h"
#include "cpu/instr.h"

/*
Put the implementations of `int' instructions here.

Special note for `int': please use the instruction name `int_' instead of `int'.
*/

make_instr_func(int_){
	decode_operand_i
	decode_data_size_b
	operand_read(&opr_src);
	print_asm_1("int", "", 2, &opr_src);
	raise_sw_intr(opr_src.val);
	return 0;
}
