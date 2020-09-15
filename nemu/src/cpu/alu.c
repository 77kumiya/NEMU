#include "cpu/cpu.h"

#ifndef NEMU_REF_ALU

#define cutx(x, size) ((~((0xffffffff << ((size) - 1)) << 1)) & (x))	// return the lower <size> bits of x.
#define signx(x, size) (!!((1u << ((size) - 1)) & (x)))	// return the sign bit of x, a integer of <size> bits length.

inline void alu_set_SF_ZF_PF(uint32_t res, size_t data_size)
{
	cpu.eflags.SF = signx(res, data_size);
	cpu.eflags.ZF = cutx(res, data_size) == 0;
	
	uint8_t pf_t = 1;
	int i;
	for(i=0; i<8; ++i) pf_t ^= (res>>i)&1;
	cpu.eflags.PF = pf_t;
}

inline void alu_set_ADD_OF(uint32_t res, uint32_t src, uint32_t dest, size_t data_size)
{
	cpu.eflags.OF = signx(src, data_size) == signx(dest, data_size) && signx(res, data_size) != signx(src, data_size);
}

inline void alu_set_ADD_CF(uint32_t res, uint32_t src, size_t data_size)
{
	cpu.eflags.CF = cutx(res, data_size) < cutx(src, data_size);
}

inline void alu_set_ADC_OF(uint32_t res, uint32_t src, uint32_t dest, size_t data_size)
{
	cpu.eflags.OF = signx(src, data_size) == signx(dest, data_size) && signx(res, data_size) != signx(src, data_size);
}

inline void alu_set_ADC_CF(uint32_t res, uint32_t src, size_t data_size)
{
	cpu.eflags.CF = cutx(res, data_size) < cutx(src, data_size) || (cutx(res, data_size) == cutx(src, data_size) && cpu.eflags.CF == 1);
}

inline void alu_set_SUB_OF(uint32_t res, uint32_t src, uint32_t dest, size_t data_size)
{
	cpu.eflags.OF = signx(src, data_size) != signx(dest, data_size) && signx(res, data_size) != signx(dest, data_size);
}

inline void alu_set_SUB_CF(uint32_t src, uint32_t dest, size_t data_size)
{
	cpu.eflags.CF = cutx(dest, data_size) < cutx(src, data_size);
}

inline void alu_set_SBB_OF(uint32_t res, uint32_t src, uint32_t dest, size_t data_size)
{
	cpu.eflags.OF = signx(src, data_size) != signx(dest, data_size) && signx(res, data_size) != signx(dest, data_size);
}

inline void alu_set_SBB_CF(uint32_t src, uint32_t dest, size_t data_size)
{
	cpu.eflags.CF = cutx(dest, data_size) < cutx(src, data_size) || (cutx(dest, data_size) == cutx(src, data_size) && cpu.eflags.CF == 1);
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
	al_set_SF_ZF_PF(res, data_size);
	alu_set_SBB_CF(src, dest, data_size);
	return cutx(res, data_size);
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}
