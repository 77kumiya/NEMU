#include "memory/mmu/cache.h"
#define CACHE_LINE_NUM 1024

extern uint8_t *hw_mem;
struct CacheLine cache[CACHE_LINE_NUM];

static inline uint32_t get_inblock_addr(paddr_t paddr);
static inline uint32_t get_group_num(paddr_t paddr);
static inline uint32_t get_label(paddr_t paddr);
static inline void parse_paddr(paddr_t paddr, uint32_t *p_label, uint32_t *p_group_num, uint32_t *p_inblock_addr);
static uint32_t cross_line_read(paddr_t paddr, size_t len, uint32_t inblock_addr);
static uint32_t search_in_group(uint32_t group_num, uint32_t label);
static uint32_t choose_slot(uint32_t group_num);
static void load_data_from_memory(uint32_t label, uint32_t group_num);
static void cross_line_write(paddr_t paddr, size_t len, uint32_t inblock_addr, uint32_t data);

// init the cache
void init_cache()
{
	int i;
	for(i = 0; i < CACHE_LINE_NUM; ++i)
	{
	    cache[i].valid = false;
	}
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{
	// parse paddr to get label, group number and in-block address
    uint32_t label, group_num, inblock_addr;
	parse_paddr(paddr, &label, &group_num, &inblock_addr);
	// handle situations when data cross line
	if(inblock_addr + len > 64)
	{
	    cross_line_write(paddr, len, inblock_addr, data);
	    return;
	}
	// search in group
	uint32_t i = search_in_group(group_num, label);
	// if cache hit, write on cache
	if(i < group_num * 8 + 8){
	    memcpy(cache[i].data + inblock_addr, &data, len);
	}
	// write on memory
	memcpy(hw_mem + paddr, &data, len);
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{
    // parse paddr to get label, group number and in-block address
    uint32_t label, group_num, inblock_addr;
	parse_paddr(paddr, &label, &group_num, &inblock_addr);
	// handle situations when data cross line
	if(inblock_addr + len > 64)
	{
	    return cross_line_read(paddr, len, inblock_addr);
	}
	// search in group
	uint32_t i = search_in_group(group_num, label);
	// if cache miss, load data from memory
	if(i == group_num * 8 + 8){
	    load_data_from_memory(label, group_num);
	    return cache_read(paddr, len);
	}
	uint32_t ret = 0;
	memcpy(&ret, cache[i].data + inblock_addr, len);
	return ret;
}

static inline uint32_t get_inblock_addr(paddr_t paddr)
{
    uint32_t result = 0x3f & paddr;
    assert(result < 64 && result >= 0);
    return result;
}

static inline uint32_t get_group_num(paddr_t paddr)
{
    uint32_t result = (0x1fc0 & paddr) >> 6;
    assert(result < 128 && result >= 0);
    return result;
}

static inline uint32_t get_label(paddr_t paddr)
{
    return 0xffffe000 & paddr;
}

static inline void parse_paddr(paddr_t paddr, uint32_t *p_label, uint32_t *p_group_num, uint32_t *p_inblock_addr)
{
    *p_label = get_label(paddr);
	*p_group_num = get_group_num(paddr);
	*p_inblock_addr = get_inblock_addr(paddr);
}

static uint32_t cross_line_read(paddr_t paddr, size_t len, uint32_t inblock_addr)
{
    size_t len1 = 64 - inblock_addr;
	size_t len2 = len - len1;
	uint32_t data1 = cache_read(paddr, len1);
	uint32_t data2 = cache_read(paddr + len1, len2);
	return (data2 << (len1 * 8)) | data1;
}

static uint32_t search_in_group(uint32_t group_num, uint32_t label)
{
    uint32_t group_start = group_num * 8;
	uint32_t group_end = group_start + 8;
	uint32_t i;
	for(i = group_start; i < group_end; ++i)
	{
	    if(cache[i].valid && cache[i].label == label)
	    {
	        break;
	    }
	}
	return i;
}

static uint32_t choose_slot(uint32_t group_num)
{
    uint32_t group_start = group_num * 8;
	uint32_t group_end = group_start + 8;
	uint32_t i;
	for(i = group_start; i < group_end; ++i)
	{
	    // stop at the first available slot
	    if(!cache[i].valid)
	    {
	        break;
	    }
	}
	// if no available slot, return the first slot
	return (i == group_end) ? group_start : i;
}

static void load_data_from_memory(uint32_t label, uint32_t group_num)
{
    // choose a slot
    uint32_t slot_idx = choose_slot(group_num);
    cache[slot_idx].valid = true;
    cache[slot_idx].label = label;
    memcpy(cache[slot_idx].data, hw_mem + (label | (group_num << 6)), 64);
}

static void cross_line_write(paddr_t paddr, size_t len, uint32_t inblock_addr, uint32_t data)
{
    size_t len1 = 64 - inblock_addr;
	size_t len2 = len - len1;
	uint32_t data1 = ((~(0u)) >> ((4 - len1) << 3)) & data;
	uint32_t data2 = data >> (len1 << 3);
	cache_write(paddr, len1, data1);
	cache_write(paddr + len1, len2, data2);
}