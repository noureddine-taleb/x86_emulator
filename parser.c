#include "common.h"
#include "instruction.h"
#include "parser.h"
#include "memory.h"

union x86_instruction_flags chk[MAX_OPCODE];
bool chsz_ad;

void set_chsz_ad(bool ad)
{
	chsz_ad = ad;
}

u8 parse_prefix(void)
{
	u8 code, chsz = 0;

	while (true)
	{
		code = get_code8(0);
		switch (code)
		{
		case 0x26:
			PRE_SEGMENT = ES;
			goto set_pre;
		case 0x2e:
			PRE_SEGMENT = CS;
			goto set_pre;
		case 0x36:
			PRE_SEGMENT = SS;
			goto set_pre;
		case 0x3e:
			PRE_SEGMENT = DS;
			goto set_pre;
		case 0x64:
			PRE_SEGMENT = FS;
			goto set_pre;
		case 0x65:
			PRE_SEGMENT = GS;
			goto set_pre;
		case 0x66:
			chsz |= CHSZ_OP;
			goto next;
		case 0x67:
			chsz |= CHSZ_AD;
			goto next;
		case 0xf2:
			PRE_REPEAT = REPNZ;
			goto next;
		case 0xf3:
			PRE_REPEAT = REPZ;
			goto next;
		default:
			return chsz;

		set_pre:
			PREFIX = code;
		next:
			UPDATE_EIP(1);
			break;
		}
	}
}

void parse(void)
{
	u16 opcode;

	parse_opcode();

	opcode = OPCODE;
	if (opcode >> 8 == 0x0f)
		opcode = (opcode & 0xff) | 0x0100;

	if (chk[opcode].modrm)
		parse_modrm_sib_disp();

	if (chk[opcode].imm32)
	{
		IMM32 = get_code32(0);
		DEBUG("imm32:0x%08x ", IMM32);
		UPDATE_EIP(4);
	}
	else if (chk[opcode].imm16)
	{
		IMM16 = get_code16(0);
		DEBUG("imm16:0x%04x ", IMM16);
		UPDATE_EIP(2);
	}
	else if (chk[opcode].imm8)
	{
		IMM8 = (s8)get_code8(0);
		DEBUG("imm8:0x%02x ", IMM8);
		UPDATE_EIP(1);
	}
	if (chk[opcode].ptr16)
	{
		PTR16 = get_code16(0);
		DEBUG("ptr16:0x%04x", PTR16);
		UPDATE_EIP(2);
	}

	if (chk[opcode].moffs)
		parse_moffs();
}

void parse_opcode(void)
{
	OPCODE = get_code8(0);
	UPDATE_EIP(1);

	// two byte opcode
	if (OPCODE == 0x0f)
	{
		OPCODE = (OPCODE << 8) + get_code8(0);
		UPDATE_EIP(1);
	}

	if (is_mode32())
		DEBUG("CS:%04x EIP:0x%04x opcode:%02x ", get_segment(CS), GET_EIP() - 1, OPCODE);
	else
		DEBUG("CS:%04x  IP:0x%04x opcode:%02x ", get_segment(CS), GET_IP() - 1, OPCODE);
}

void parse_modrm_sib_disp(void)
{
	_MODRM = get_code8(0);
	UPDATE_EIP(1);

	DEBUG("[mod:0x%02x reg:0x%02x rm:0x%02x] ", MOD, REG, RM);

	if (is_mode32() ^ chsz_ad)
		parse_modrm32();
	else
		parse_modrm16();
}

void parse_modrm32(void)
{
	if (MOD != 3 && RM == 4)
	{
		_SIB = get_code8(0);
		UPDATE_EIP(1);
		DEBUG("[scale:0x%02x index:0x%02x base:0x%02x] ", SCALE, INDEX, BASE);
	}

	if (MOD == 2 || (MOD == 0 && RM == 5) || (MOD == 0 && BASE == 5))
	{
		DISP32 = get_code32(0);
		UPDATE_EIP(4);
		DEBUG("disp32:0x%08x ", DISP32);
	}
	else if (MOD == 1)
	{
		DISP8 = (s8)get_code8(0);
		UPDATE_EIP(1);
		DEBUG("disp8:0x%02x ", DISP8);
	}
}

void parse_modrm16(void)
{
	if ((MOD == 0 && RM == 6) || MOD == 2)
	{
		DISP16 = get_code32(0);
		UPDATE_EIP(2);
		DEBUG("disp16:0x%04x ", DISP16);
	}
	else if (MOD == 1)
	{
		DISP8 = (s8)get_code8(0);
		UPDATE_EIP(1);
		DEBUG("disp8:0x%02x ", DISP8);
	}
}

void parse_moffs(void)
{
	if (is_mode32() ^ chsz_ad)
	{
		MOFFS = get_code32(0);
		UPDATE_EIP(4);
	}
	else
	{
		MOFFS = get_code16(0);
		UPDATE_EIP(2);
	}
	DEBUG("moffs:0x%04x ", MOFFS);
}
