#include "cpu/cpu.h"

#ifndef NEMU_REF_ALU

#define bitMask(size) (1u << ((size) - 1))	// return a integer whose size-th (1 indexed) lower bit is 1 and others are 0s.
#define signx(x, size) (!!(bitMask(size) & (x)))	// return the sign bit of x, a integer of <size> bits length.
#define mask(size) ((0xffffffffu) >> (32 - (size)))	//return a integer whose lower <size> bits are 1s and others are 0s.
#define mask64(size) ((0xffffffffffffffffu) >> (64 - (size)))
#define cutx(x, size) (mask(size) & (x))	// return the lower <size> bits of x.

inline void alu_set_SF_ZF_PF(uint32_t res, size_t data_size)
{
	cpu.eflags.SF = signx(res, data_size);
	cpu.eflags.ZF = cutx(res, data_size) == 0;
	
	uint8_t pf_t = 1;
#if 1
	pf_t ^= res & 1;
	res = res >> 1;
	pf_t ^= res & 1;
	res = res >> 1;
	pf_t ^= res & 1;
	res = res >> 1;
	pf_t ^= res & 1;
	res = res >> 1;
	pf_t ^= res & 1;
	res = res >> 1;
	pf_t ^= res & 1;
	res = res >> 1;
	pf_t ^= res & 1;
	res = res >> 1;
	pf_t ^= res & 1;
#else
    // Another implementation
	int i;
	for(i=0; i<8; ++i)
	{
		pf_t ^= res & 1;
		res = res >> 1;
	}
#endif
	cpu.eflags.PF = pf_t;
}

inline void alu_set_ADD_OF(uint32_t res, uint32_t src, uint32_t dest, size_t data_size)
{
    src = signx(src, data_size);
	cpu.eflags.OF = src == signx(dest, data_size) && signx(res, data_size) != src;
}

inline void alu_set_ADD_CF(uint32_t res, uint32_t src, size_t data_size)
{
	cpu.eflags.CF = cutx(res, data_size) < cutx(src, data_size);
}

inline void alu_set_ADC_OF(uint32_t res, uint32_t src, uint32_t dest, size_t data_size)
{
    src = signx(src, data_size);
	cpu.eflags.OF = src == signx(dest, data_size) && signx(res, data_size) != src;
}

inline void alu_set_ADC_CF(uint32_t res, uint32_t src, size_t data_size)
{
    src = cutx(src, data_size);
    res = cutx(res, data_size);
	cpu.eflags.CF = res < src || (res == src && cpu.eflags.CF == 1);
}

inline void alu_set_SUB_OF(uint32_t res, uint32_t src, uint32_t dest, size_t data_size)
{
    dest = signx(dest, data_size);
	cpu.eflags.OF = signx(src, data_size) != dest && signx(res, data_size) != dest;
}

inline void alu_set_SUB_CF(uint32_t src, uint32_t dest, size_t data_size)
{
	cpu.eflags.CF = cutx(dest, data_size) < cutx(src, data_size);
}

inline void alu_set_SBB_OF(uint32_t res, uint32_t src, uint32_t dest, size_t data_size)
{
    dest = signx(dest, data_size);
	cpu.eflags.OF = signx(src, data_size) != dest && signx(res, data_size) != dest;
}

inline void alu_set_SBB_CF(uint32_t src, uint32_t dest, size_t data_size)
{
    src = cutx(src, data_size);
    dest = cutx(dest, data_size);
	cpu.eflags.CF = dest < src || (dest == src && cpu.eflags.CF == 1);
}

#endif

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	uint32_t res = src + dest;
	alu_set_ADD_OF(res, src, dest, data_size);
	alu_set_SF_ZF_PF(res, data_size);
	alu_set_ADD_CF(res, src, data_size);
	return cutx(res, data_size);
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	uint32_t res = src + dest + cpu.eflags.CF;
	alu_set_ADC_OF(res, src, dest, data_size);
	alu_set_SF_ZF_PF(res, data_size);
	alu_set_ADC_CF(res, src, data_size);
	return cutx(res, data_size);
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	uint32_t res = dest - src;
	alu_set_SUB_OF(res, src, dest, data_size);
	alu_set_SF_ZF_PF(res, data_size);
	alu_set_SUB_CF(src, dest, data_size);
	return cutx(res, data_size);
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	uint32_t res = dest - src - cpu.eflags.CF;
	alu_set_SBB_OF(res, src, dest, data_size);
	alu_set_SF_ZF_PF(res, data_size);
	alu_set_SBB_CF(src, dest, data_size);
	return cutx(res, data_size);
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	uint64_t res = ((uint64_t)src) * ((uint64_t)dest);
	cpu.eflags.OF = !!((mask64(data_size) ^ mask64(data_size << 1)) & res);
	cpu.eflags.CF = cpu.eflags.OF;
	return res;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	//OF and CF are ignored.
	return ((int64_t)src) * ((int64_t)dest);
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	return dest / src;
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	return dest / src;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	return dest % src;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	return dest % src;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	uint32_t res = src & dest;
	cpu.eflags.OF = 0;
	alu_set_SF_ZF_PF(res, data_size);
	cpu.eflags.CF = 0;
	return cutx(res, data_size);
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	uint32_t res = src ^ dest;
	cpu.eflags.OF = 0;
	alu_set_SF_ZF_PF(res, data_size);
	cpu.eflags.CF = 0;
	return cutx(res, data_size);
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	uint32_t res = src | dest;
	cpu.eflags.OF = 0;
	alu_set_SF_ZF_PF(res, data_size);
	cpu.eflags.CF = 0;
	return cutx(res, data_size);
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	src &= 0x1f;
	uint32_t msk = mask(data_size);
	uint32_t res = ((dest & msk) << src) & msk;
	//cpu.eflags.OF is ignored.
	alu_set_SF_ZF_PF(res, data_size);
	cpu.eflags.CF = (data_size < src) ? 0 : !!(bitMask(data_size - src + 1) & dest);
	return res;
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	src &= 0x1f;
	uint32_t msk = mask(data_size);
	uint32_t res = ((dest & msk) >> src) & msk;
	//cpu.eflags.OF is ignored.
	alu_set_SF_ZF_PF(res, data_size);
	cpu.eflags.CF = (data_size < src) ? 0 : !!(bitMask(src) & dest);
	return res;
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	src &= 0x1f;
	uint32_t msk = mask(data_size);
#if 0
    // Another implementation
	uint32_t res = (((dest & msk) >> src) & msk) | (signx(dest, data_size) == 0 ? 0 : (msk ^ (data_size < src ? 0 : mask(data_size - src))));
#else
	uint32_t res;
	switch(data_size >> 3)
	{
		case 1 : res = ((int32_t)(int8_t)dest) >> (int32_t)src; break;
		case 2 : res = ((int32_t)(int16_t)dest) >> (int32_t)src; break;
		case 4 : res = ((int32_t)dest) >> (int32_t)src; break;
		default : assert(0);
	}
	res &= msk;
#endif
	//cpu.eflags.OF is ignored.
	alu_set_SF_ZF_PF(res, data_size);
	cpu.eflags.CF = (data_size < src) ? signx(dest, data_size) : !!(bitMask(src) & dest);
	return res;
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	return alu_shl(src, dest, data_size);
#endif
}
