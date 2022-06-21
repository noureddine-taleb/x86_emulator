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

u8 get_r8(void);
u64 calc_modrm(void);
void set_rm8(u8 v);
enum sgreg select_segment(void);

extern struct x86_instruction inst;

#endif // _INSTRUCTION_H
