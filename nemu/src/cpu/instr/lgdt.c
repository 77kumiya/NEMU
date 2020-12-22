#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/

make_instr_func(lgdt){
    int len = 2;
    uint32_t gdt_addr = instr_fetch(eip + 2, 4);
    cpu.gdtr.limit = paddr_read(gdt_addr, 2);
    cpu.gdtr.base = paddr_read(gdt_addr + 2, 4);
    len += 4;
    opr_src.type = OPR_MEM;
    opr_src.addr = gdt_addr;
    opr_src.data_size = 32;
    opr_src.sreg = SREG_DS;
    print_asm_1("lgdt", "", len, &opr_src);
    return len;
}