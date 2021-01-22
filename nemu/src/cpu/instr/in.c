#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `in' instructions here.
*/

make_instr_func(in_b){
	cpu.gpr[REG_AL]._8[0] = pio_read(cpu.gpr[REG_DX]._16, 1);
	return 1;
}

make_instr_func(in_v){
	uint32_t len = data_size >> 3;
	(len == 2 ? cpu.gpr[REG_AX]._16 : cpu.eax) = pio_read(cpu.gpr[REG_DX]._16, len);
	return 1;
}

