#include "cpu/instr.h"

make_instr_func(jmp_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);

        cpu.eip += offset;

        return 1 + data_size / 8;
}

make_instr_func(jmp_short)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = 8;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, 8);
        
        print_asm_1("jmp", "", 2, &rel);

        cpu.eip += offset;

        return 2;
}

make_instr_func(jmp_rm_v)
{
        int len = 1;
        decode_data_size_v
        decode_operand_rm

        operand_read(&opr_src);
        
        print_asm_1("jmp", "", len, &opr_src);

        cpu.eip = opr_src.val;

        return 0;
}

make_instr_func(ljmp)
{
        int len = 1;
        uint32_t eaddr = instr_fetch(eip + 1, data_size >> 3);
        uint16_t selector = instr_fetch(eip + 1 + (data_size >> 3), 2);
        cpu.cs.val = selector;
        load_sreg((uint8_t)cpu.cs.index);
        
        // print asm
        len += 2 + (data_size >> 3);
        opr_src.type = OPR_IMM;
        opr_src.data_size = 16;
        opr_src.val = selector;
        opr_dest.type = OPR_IMM;
        opr_dest.data_size = data_size;
        opr_dest.val = eaddr;
        print_asm_2("ljmp", "", len, &opr_src, &opr_dest);

        cpu.eip = eaddr;

        return 0;
}
