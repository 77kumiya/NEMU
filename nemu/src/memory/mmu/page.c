#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
    union {
        struct {
            uint32_t offset : 12;
            uint32_t page : 10;
            uint32_t dir : 10;
        };
        uint32_t val;
    } laddr_parser;
    laddr_parser.val = (uint32_t)laddr;
    
    paddr_t pdb = ((uint32_t)cpu.cr3.page_directory_base << 12);
    
    PDE pde;
    pde.val = paddr_read(pdb + (laddr_parser.dir << 2), 4);
    assert(pde.present == 1);
    
    PTE pte;
    pte.val = paddr_read(((uint32_t)pde.page_frame << 12) + (laddr_parser.page << 2), 4);
    assert(pte.present == 1);
    
    return ((uint32_t)pte.page_frame << 12) | laddr_parser.offset;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
