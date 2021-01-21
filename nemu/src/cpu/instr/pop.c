#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/

#define dest_prep               \
    opr_dest.type = OPR_MEM;    \
    opr_dest.addr = cpu.esp;    \
    opr_dest.sreg = SREG_SS;

static void instr_execute_1op(){
    dest_prep
    operand_read(&opr_dest);
    cpu.esp += opr_src.data_size / 8;
    opr_src.val = opr_dest.val;
    operand_write(&opr_src);
}

static uint32_t pop(){
	uint32_t ret = vaddr_read(cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	return ret;
}

make_instr_impl_1op(pop, r, v)
make_instr_impl_1op(pop, rm, v)
make_instr_func(popa){
	cpu.edi = pop();
	cpu.esi = pop();
	cpu.ebp = pop();
	pop();
	cpu.ebx = pop();
	cpu.edx = pop();
	cpu.ecx = pop();
	cpu.eax = pop();
	return 1;
}
