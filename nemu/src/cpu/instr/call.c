#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/

make_instr_func(call)
{
        OPERAND rel, dest;
        
        cpu.esp -= data_size / 8;
        dest.type = OPR_MEM;
        dest.sreg = SREG_SS;
        dest.data_size = data_size;
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
