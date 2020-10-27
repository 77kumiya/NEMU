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

make_instr_func(ret_i)
{
    int len = 1;
    OPERAND pre_addr;
    pre_addr.type = OPR_MEM;
    pre_addr.data_size = 32;
    pre_addr.addr = cpu.esp;
    pre_addr.sreg = SREG_SS;
    operand_read(&pre_addr);
    cpu.esp += 4;
    
    decode_data_size_w
    decode_operand_i
    operand_read(&opr_src);
    cpu.esp += opr_src.val << (data_size >> 5);
    
    print_asm_1("ret", "", len, &opr_src);
    
    cpu.eip = pre_addr.val;
    
    return 0;
}