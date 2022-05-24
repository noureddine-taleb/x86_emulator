#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include "common.h"
#include "cpu.h"

#define PREFIX (inst.prefix)
#define OPCODE (inst.opcode)
#define _MODRM (inst._modrm)
#define MOD (inst.modrm.mod)
#define REG (inst.modrm.reg)
#define RM (inst.modrm.rm)
#define _SIB (inst._sib)
#define SCALE (inst.sib.scale)
#define INDEX (inst.sib.index)
#define BASE (inst.sib.base)
#define DISP32 (inst.disp32)
#define DISP16 (inst.disp16)
#define DISP8 (inst.disp8)
#define IMM32 (inst.imm32)
#define IMM16 (inst.imm16)
#define IMM8 (inst.imm8)
#define PTR16 (inst.ptr16)
#define MOFFS (inst.moffs)
#define PRE_SEGMENT (inst.pre_segment)
#define PRE_REPEAT (inst.pre_repeat)
#define SEGMENT (inst.segment)

#define MAX_OPCODE 0x200

#define CHK_MODRM (1 << 0)
#define CHK_IMM32 (1 << 1)
#define CHK_IMM16 (1 << 2)
#define CHK_IMM8 (1 << 3)
#define CHK_PTR16 (1 << 4)
#define CHK_MOFFS (1 << 5)

#define CHSZ_NONE 0
#define CHSZ_OP 1
#define CHSZ_AD 2

enum rep
{
	NONE,
	REPZ,
	REPNZ
};

struct modrm
{
	u8 rm : 3;
	u8 reg : 3;
	u8 mod : 2;
};

struct sib
{
	u8 base : 3;
	u8 index : 3;
	u8 scale : 2;
};

struct x86_instruction
{
	u16 prefix;
	enum sgreg pre_segment;
	enum rep pre_repeat;

	enum sgreg segment;
	u16 opcode;
	union
	{
		u8 _modrm;
		struct modrm modrm;
	};
	union
	{
		u8 _sib;
		struct sib sib;
	};
	union
	{
		s8 disp8;
		s16 disp16;
		s32 disp32;
	};
	union
	{
		s8 imm8;
		s16 imm16;
		s32 imm32;
	};
	s16 ptr16;
	u32 moffs;
};

typedef void (*instruction_handler_t)(void);

extern instruction_handler_t instructions[MAX_OPCODE];

bool exec(void);

void add_rm8_r8(void);
void add_r8_rm8(void);
void add_al_imm8(void);
void or_rm8_r8(void);
void or_r8_rm8(void);
void or_al_imm8(void);
void and_rm8_r8(void);
void and_r8_rm8(void);
void and_al_imm8(void);
void sub_rm8_r8(void);
void sub_r8_rm8(void);
void sub_al_imm8(void);
void xor_rm8_r8(void);
void xor_r8_rm8(void);
void xor_al_imm8(void);
void cmp_rm8_r8(void);
void cmp_r8_rm8(void);
void cmp_al_imm8(void);
void jo_rel8(void);
void jno_rel8(void);
void jb_rel8(void);
void jnb_rel8(void);
void jz_rel8(void);
void jnz_rel8(void);
void jbe_rel8(void);
void ja_rel8(void);
void js_rel8(void);
void jns_rel8(void);
void jp_rel8(void);
void jnp_rel8(void);
void jl_rel8(void);
void jnl_rel8(void);
void jle_rel8(void);
void jnle_rel8(void);
void test_rm8_r8(void);
void xchg_r8_rm8(void);
void mov_rm8_r8(void);
void mov_r8_rm8(void);
void mov_sreg_rm16(void);
void nop(void);
void mov_al_moffs8(void);
void mov_moffs8_al(void);
void test_al_imm8(void);
void mov_r8_imm8(void);
void mov_rm8_imm8(void);
void retf(void);
void int3(void);
void int_imm8(void);
void iret(void);
void in_al_imm8(void);
void out_imm8_al(void);
void jmp(void);
void in_al_dx(void);
void out_dx_al(void);
void cli(void);
void sti(void);
void cld(void);
void std(void);
void hlt(void);
void mov_r32_crn(void);
void mov_crn_r32(void);
void seto_rm8(void);
void setno_rm8(void);
void setb_rm8(void);
void setnb_rm8(void);
void setz_rm8(void);
void setnz_rm8(void);
void setbe_rm8(void);
void seta_rm8(void);
void sets_rm8(void);
void setns_rm8(void);
void setp_rm8(void);
void setnp_rm8(void);
void setl_rm8(void);
void setnl_rm8(void);
void setle_rm8(void);
void setnle_rm8(void);
void code_80(void);
void code_82(void);
void code_c0(void);
void code_f6(void);

extern struct x86_instruction inst;

#endif // _INSTRUCTION_H
