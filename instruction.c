#include "common.h"
#include "exec.h"
#include "instruction.h"
#include "memory.h"
#include "parser.h"

struct x86_instruction inst;

u8 get_r8(void)
{
	return get_gpreg8(REG);
}

u64 calc_modrm(void)
{
	ASSERT(MOD != 3);

	SEGMENT = DS;
	if (is_mode32() ^ chsz_ad)
		return calc_modrm32();
	else
		return calc_modrm16();
}

u64 calc_modrm16(void)
{
	u64 addr = 0;

	switch (MOD)
	{
	case 1:
		addr += DISP8;
		break;
	case 2:
		addr += DISP16;
		break;
	}

	switch (RM)
	{
	case 0:
	case 1:
	case 7:
		addr += GET_GPREG(BX);
		break;
	case 2:
	case 3:
	case 6:
		if (MOD == 0 && RM == 6)
			addr += DISP16;
		else
		{
			addr += GET_GPREG(BP);
			SEGMENT = SS;
		}
		break;
	}

	if (RM < 6)
	{
		if (RM % 2)
			addr += GET_GPREG(DI);
		else
			addr += GET_GPREG(SI);
	}

	return addr;
}

u64 calc_modrm32(void)
{
	u64 addr = 0;

	switch (MOD)
	{
	case 1:
		addr += DISP8;
		break;
	case 2:
		addr += DISP32;
		break;
	}

	switch (RM)
	{
	case 4:
		addr += calc_sib();
		break;
	case 5:
		if (MOD == 0)
		{
			addr += DISP32;
			break;
		}
	default:
		SEGMENT = (RM == 5) ? SS : DS;
		addr += GET_GPREG((enum reg32)RM);
	}

	return addr;
}

void set_rm8(u8 v)
{
	if (MOD == 3)
	{
		set_gpreg8(RM, v);
	}
	else
	{
		WRITE_MEM8(calc_modrm(), v);
	}
}

enum sgreg select_segment(void)
{
	return PREFIX ? PRE_SEGMENT : SEGMENT;
}
