#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
    assert(cpu.segReg[sreg].base == 0x0);
	return offset + cpu.segReg[sreg].base;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	uint32_t desc_linear_addr = cpu.gdtr.base + (cpu.segReg[sreg].index << 3);
	SegDesc sreg_desc;
    sreg_desc.val[0] = laddr_read(desc_linear_addr, 4);
    sreg_desc.val[1] = laddr_read(desc_linear_addr + 4, 4);
    cpu.segReg[sreg].base = (sreg_desc.base_31_24 << 24) + (sreg_desc.base_23_16 << 16) + (sreg_desc.base_15_0);
    cpu.segReg[sreg].limit = (sreg_desc.limit_19_16 << 16) + (sreg_desc.limit_15_0);
    cpu.segReg[sreg].type = sreg_desc.type;
    cpu.segReg[sreg].privilege_level = sreg_desc.privilege_level;
    cpu.segReg[sreg].soft_use = sreg_desc.soft_use;
    assert(cpu.segReg[sreg].base == 0x0 && cpu.segReg[sreg].limit == 0x000fffff && sreg_desc.granularity == 1);
}
