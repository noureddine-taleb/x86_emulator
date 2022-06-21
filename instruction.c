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

u32 calc_sib(void)
{
	u32 base;

	if (BASE == 5 && MOD == 0)
		base = DISP32;
	else if (BASE == 4)
	{
		if (SCALE == 0)
		{ // BASE == 4, INDEX ==4, SCALE == 0 : [esp]
			SEGMENT = SS;
			base = 0;
		}
		else
			ERROR("not implemented SIB (base = %d, index = %d, scale = %d)\n", BASE, INDEX, SCALE);
	}
	else
	{
		SEGMENT = (RM == 5) ? SS : DS;
		base = get_gpreg32(BASE);
	}

	return base + get_gpreg32(INDEX) * (1 << SCALE);
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
		addr += get_gpreg16(BX);
		break;
	case 2:
	case 3:
	case 6:
		if (MOD == 0 && RM == 6)
			addr += DISP16;
		else
		{
			addr += get_gpreg16(BP);
			SEGMENT = SS;
		}
		break;
	}

	if (RM < 6)
	{
		if (RM % 2)
			addr += get_gpreg16(DI);
		else
			addr += get_gpreg16(SI);
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
		addr += get_gpreg32((enum reg32)RM);
	}

	return addr;
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
