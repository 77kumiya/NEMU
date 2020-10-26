#include "cpu/instr.h"
/*
Put the implementations of `ret' instructions here.
*/
make_instr_func(ret)
{
    OPERAND pre_addr;
    pre_addr.type = OPR_MEM;
    pre_addr.data_size = 32;
    pre_addr.addr = cpu.esp;
    pre_addr.sreg = SREG_SS;
    
    operand_read(&pre_addr);
    cpu.esp += 4;
    
    print_asm_0("ret", "", 1);
    
    cpu.eip = pre_addr.val;
    
    return 0;
}