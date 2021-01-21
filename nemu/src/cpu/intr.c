#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

static void push(uint32_t data){
	cpu.esp -= 4;
	vaddr_write(cpu.esp, SREG_SS, 4, data);
}

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
	// push eflags, cs and eip onto stack
        push(cpu.eflags.val);
	push(cpu.cs.val);
	push(cpu.eip);
	// find the IDT entry
	laddr_t idt_entry_addr = cpu.idtr.base + (intr_no << 3);
	GateDesc idt_entry;
       	idt_entry.val[0] = laddr_read(idt_entry_addr, 4);
	idt_entry.val[1] = laddr_read(idt_entry_addr + 4, 4);
	assert(idt_entry.present == 1);
	// clear IF if it is an interrupt
	if(idt_entry.type == 0xE){
		cpu.eflags.IF = 0;
	}
	// set the cs:eip to the entry of the interrupt handler
		// need to reload cs with load_sreg()
	cpu.cs.val = idt_entry.selector;
	load_sreg(idt_entry.selector >> 3);
	cpu.eip = (idt_entry.offset_31_16 << 16) | idt_entry.offset_15_0;
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
