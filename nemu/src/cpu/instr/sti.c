#include "cpu/instr.h"
/*
Put the implementations of `sti' instructions here.
*/
make_instr_func(sti){
	int len = 1;
	cpu.eflags.IF = 1;
	print_asm_0("sti", "", len);
	return len;
}
