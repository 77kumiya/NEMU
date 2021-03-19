#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE) // number of page tables to cover the vmem

PDE *get_updir();
PTE table[1024] align_to_page;

void create_video_mapping()
{

	/* TODO: create an identical mapping from virtual memory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memeory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
    /*
	PDE *pdir = get_updir();
	PDE *pde = pdir + 0;
	assert(pde->present == 1);
	PTE *ptbl = (PTE *)pa_to_va((uint32_t)pde->page_frame << 12);
	int i;
	int vmem_pte_base_idx = 0x0a0;
	for(i = 0; i < 16; ++i){
		PTE *pte = ptbl + vmem_pte_base_idx + i;
		pte->val = make_pte((vmem_pte_base_idx + i) << 12);
	}
    */
    PDE *pdir = (PDE *)va_to_pa(get_updir());
	PTE *ptable = (PTE *)va_to_pa(table);
	pdir[0].val = make_pde(ptable); 
	ptable += 0xa0;
	for(uint32_t i = 0xa0; i <= 0xaf; i ++) {
		ptable->val = make_pte(i << 12);
		ptable ++;
	}
}

void video_mapping_write_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		buf[i] = i;
	}
}

void video_mapping_read_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		assert(buf[i] == i);
	}
}

void video_mapping_clear()
{
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}
