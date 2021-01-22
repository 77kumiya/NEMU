#include "cpu/instr.h"
/*
Put the implementations of `iret' instructions here.
*/

static uint32_t pop(){
	uint32_t ret = vaddr_read(cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	return ret;
}

make_instr_func(iret){
	cpu.eip = pop();
	cpu.cs.val = (uint16_t)pop();
	load_sreg(1);
	cpu.eflags.val = pop();
	return 0;
}

