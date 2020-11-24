#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/

make_instr_func(call)
{
        OPERAND rel, dest;
        
        cpu.esp -= 4;
        dest.type = OPR_MEM;
        dest.sreg = SREG_SS;
        dest.data_size = 32;
        dest.addr = cpu.esp;
        dest.val = eip + 1 + data_size / 8;
        operand_write(&dest);
        
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;
        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        print_asm_1("call", "", 1 + data_size / 8, &rel);
        cpu.eip += offset;
        
        return 1 + data_size / 8;
}


make_instr_func(call_rm_v)
{   
    int len = 1;
    decode_data_size_v
    decode_operand_rm
    operand_read(&opr_src);
    
    cpu.esp -= 4;
    opr_dest.type = OPR_MEM;
    opr_dest.sreg = SREG_SS;
    opr_dest.addr = cpu.esp;
    opr_dest.val = eip + len;
    operand_write(&opr_dest);
    
    print_asm_1("call", "", len, &opr_src);
    
    cpu.eip = opr_src.val;
    return 0;
}