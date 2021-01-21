#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

static void push(uint32_t data, uint32_t size){
	OPERAND dest;
        cpu.esp -= 4;
        dest.type = OPR_MEM;
        dest.sreg = SREG_SS;
        dest.data_size = size;
        dest.addr = cpu.esp;
        dest.val = data;
        operand_write(&dest);
}

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
	// push eflags, cs and eip onto stack
        push(cpu.eflags.val, 32);
	push(cpu.cs.val, 32);
	push(cpu.eip, 32);
	// find the IDT entry
	GateDesc *idt_entry = (GateDesc *)(cpu.idtr.base + (intr_no << 3));
	assert(idt_entry->present == 1);
	assert(0);
	// clear IF if it is an interrupt
	if(idt_entry->type == 0xE){
		cpu.eflags.IF = 0;
	}
	// set the cs:eip to the entry of the interrupt handler
		// need to reload cs with load_sreg()
	cpu.cs.val = idt_entry->selector;
	load_sreg(idt_entry->selector >> 3);
	cpu.eip = (idt_entry->offset_31_16 << 16) | idt_entry->offset_15_0;
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
