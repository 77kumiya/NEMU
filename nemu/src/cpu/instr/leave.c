#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/

make_instr_func(leave)
{
    cpu.esp = cpu.ebp;
    
    OPERAND fetcher, ebp;
    fetcher.type = OPR_MEM;
    fetcher.addr = cpu.esp;
    fetcher.sreg = SREG_SS;
    fetcher.data_size = data_size;
    operand_read(&fetcher);
    
    ebp.type = OPR_REG;
    ebp.addr = REG_EBP;
    ebp.data_size = data_size;
    ebp.val = fetcher.val;
    operand_write(&ebp);
    
    cpu.esp += data_size / 8;
    
    print_asm_0("leave", "", 1);
    
    return 1;
}
