#ifndef _PARSER_H
#define _PARSER_H

#include "common.h"
#include "instruction.h"

union x86_instruction_flags
{
	u8 flags;
	struct
	{
		u8 modrm : 1;
		u8 imm32 : 1;
		u8 imm16 : 1;
		u8 imm8 : 1;
		u8 ptr16 : 1;
		u8 moffs : 1;
		u8 moffs8 : 1;
	};
};

extern union x86_instruction_flags chk[MAX_OPCODE];

extern bool chsz_ad;

#endif // _PARSER_H
