#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `out' instructions here.
*/

make_instr_func(out_b){
	pio_write(cpu.gpr[REG_DX]._16, 1, cpu.gpr[REG_AL]._8[0]);
	return 1;
}

make_instr_func(out_v){
	uint32_t len = data_size >> 3;
	if(len == 2){
		pio_write(cpu.gpr[REG_DX]._16, len, cpu.gpr[REG_AX]._16);
	}else{
		pio_write(cpu.gpr[REG_DX]._16, len, cpu.eax);
	}
	return 1;
}

