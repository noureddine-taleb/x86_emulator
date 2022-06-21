#include "common.h"
#include "cpu.h"
#include "exception.h"
#include "descriptor.h"
#include "memory.h"

struct x86_cpu cpu;

u32 get_eip(void)
{
	return cpu.eip;
}

u32 get_ip(void)
{
	return cpu.ip;
}

u32 get_gpreg32(enum reg32 n)
{
	ASSERT(n < GPREGS_COUNT);
	return cpu.gpregs[n].reg32;
}

u16 get_gpreg16(enum reg16 n)
{
	ASSERT(n < GPREGS_COUNT);
	return cpu.gpregs[n].reg16;
}

u8 get_gpreg8(enum reg8 n)
{
	ASSERT(n < GPREGS_COUNT);
	return n < AH ? cpu.gpregs[n].reg8_l : cpu.gpregs[n - AH].reg8_h;
}

void get_sgreg(enum sgreg n, struct sg_register *reg)
{
	ASSERT(n < SGREGS_COUNT);
	*reg = cpu.sgregs[n];
}

u32 get_dtreg_selector(enum dtreg n)
{
	ASSERT(n < DTREGS_COUNT);
	return cpu.dtregs[n].selector;
}

u32 get_dtreg_base(enum dtreg n)
{
	ASSERT(n < DTREGS_COUNT);
	return cpu.dtregs[n].base;
}

u16 get_dtreg_limit(enum dtreg n)
{
	ASSERT(n < DTREGS_COUNT);
	return cpu.dtregs[n].limit;
}

void set_eip(u32 v)
{
	cpu.eip = v;
}

void set_ip(u16 v)
{
	cpu.ip = v;
}

void set_gpreg32(enum reg32 n, u32 v)
{
	ASSERT(n < GPREGS_COUNT);
	cpu.gpregs[n].reg32 = v;
}

void set_gpreg16(enum reg16 n, u16 v)
{
	ASSERT(n < GPREGS_COUNT);
	cpu.gpregs[n].reg16 = v;
}

void set_gpreg8(enum reg8 n, u8 v)
{
	ASSERT(n < GPREGS_COUNT);
	n < AH ? (cpu.gpregs[n].reg8_l = v)
		   : (cpu.gpregs[n - AH].reg8_h = v);
}

void set_sgreg(enum sgreg n, struct sg_register *reg)
{
	ASSERT(n < SGREGS_COUNT);
	cpu.sgregs[n] = *reg;
}

void set_dtreg(enum dtreg n, u16 sel, u32 base, u16 limit)
{
	ASSERT(n < DTREGS_COUNT);
	cpu.dtregs[n].selector = sel;
	cpu.dtregs[n].base = base;
	cpu.dtregs[n].limit = limit;
}

u32 update_eip(s32 v)
{
	return cpu.eip += v;
}

u32 update_ip(s32 v)
{
	return cpu.ip += v;
}

u32 update_gpreg32(enum reg32 n, s32 v)
{
	ASSERT(n < GPREGS_COUNT);
	return cpu.gpregs[n].reg32 += v;
}

u16 update_gpreg16(enum reg16 n, s16 v)
{
	ASSERT(n < GPREGS_COUNT);
	return cpu.gpregs[n].reg16 += v;
}

u8 update_gpreg8(enum reg8 n, s8 v)
{
	ASSERT(n < GPREGS_COUNT);
	return n < AH ? (cpu.gpregs[n].reg8_l += v)
				  : (cpu.gpregs[n - AH].reg8_h += v);
}

bool is_mode32(void)
{
	return cpu.sgregs[CS].cache.flags.DB;
}

bool is_protected(void)
{
	return cpu.cr0.PE;
}

bool is_ena_paging(void)
{
	return cpu.cr0.PG;
}

u32 get_pdir_base(void)
{
	return cpu.cr3.PageDirBase;
}

u16 get_segment(enum sgreg reg)
{
	struct sg_register sg;

	get_sgreg(reg, &sg);
	return sg.raw;
}

void set_segment(enum sgreg reg, u16 sel)
{
	struct sg_register sg;
	struct sg_register_cache *cache = &sg.cache;

	get_sgreg(reg, &sg);
	sg.raw = sel;

	if (is_protected())
	{
		u32 dt_base;
		u16 dt_limit, dt_index;
		struct seg_desc gdt;
		const char *sgreg_name[] = {"ES", "CS", "SS", "DS", "FS", "GS"};

		dt_index = sg.index << 3;

		dt_base = get_dtreg_base(sg.TI ? LDTR : GDTR);
		dt_limit = get_dtreg_limit(sg.TI ? LDTR : GDTR);

		EXCEPTION(EXP_GP, (reg == CS || reg == SS) && !dt_index);
		EXCEPTION(EXP_GP, dt_index > dt_limit);

		__read_data(&gdt, dt_base + dt_index, sizeof(struct seg_desc));

		cache->base = (gdt.base_h << 24) + (gdt.base_m << 16) + gdt.base_l;
		cache->limit = (gdt.limit_h << 16) + gdt.limit_l;

		*(u8 *)&cache->flags.type = *(u8 *)&gdt.type;
		cache->flags.AVL = gdt.AVL;
		cache->flags.DB = gdt.DB;
		cache->flags.G = gdt.G;

		DEBUG("%s : dt_base=0x%04x, dt_limit=0x%02x, dt_index=0x%02x {base=0x%08x, limit=0x%08x, flags=0x%04x}", sgreg_name[reg], dt_base, dt_limit, dt_index, cache->base, cache->limit << (cache->flags.G ? 12 : 0), cache->flags.raw);
	}
	else
		cache->base = (u32)sel << 4;

	set_sgreg(reg, &sg);
}

void do_halt(bool h)
{
	cpu.__halt = h;
};

bool is_halt(void)
{
	return cpu.__halt;
}

bool is_interrupt_enabled(void)
{
	return cpu.eflags.IF;
}

u32 get_eflags(void)
{
	return cpu.eflags.reg32;
}

void set_eflags(u32 v)
{
	cpu.eflags.reg32 = v;
}

u16 get_flags(void)
{
	return cpu.eflags.reg16;
}

void set_flags(u16 v)
{
	cpu.eflags.reg16 = v;
}

void set_interrupt(bool interrupt)
{
	cpu.eflags.IF = interrupt;
}
