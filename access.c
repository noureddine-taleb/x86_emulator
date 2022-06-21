#include "common.h"
#include "cpu.h"
#include "memory.h"
#include "access.h"
#include "exception.h"

struct tlb_entry tlb[256];

bool tlb_empty(struct tlb_entry *entry)
{
	return !entry->full;
}

int tlb_find_spot(void)
{
	for (int i = 0; i < ARRAY_SIZE(tlb); i++)
	{
		if (tlb_empty(tlb + i))
			return i;
	}
	return -1;
}

bool search_tlb(u32 vpn, struct pte *pte)
{
	for (int i = 0; i < ARRAY_SIZE(tlb); i++)
	{
		if (tlb_empty(tlb + i))
			continue;

		if (tlb[i].vpn == vpn)
		{
			*pte = tlb[i].pte;
			return true;
		}
	}

	return false;
}

void cache_tlb(u32 vpn, struct pte pte)
{
	int i = tlb_find_spot();
	// TODO: this should be round robin
	if (i < 0)
		i = 0;

	tlb[i] = (struct tlb_entry){.vpn = vpn, .pte = pte, .full = true};
}

u32 get_linear(enum access_mode mode, enum sgreg seg, u32 vaddr)
{
	u32 laddr;
	u8 CPL;
	struct sg_register sg;

	CPL = get_segment(CS) & 3;
	get_sgreg(seg, &sg);

	if (is_protected())
	{
		u32 base, limit;
		struct sg_register_cache cache = sg.cache;

		base = cache.base;
		limit = cache.limit;
		if (cache.flags.G)
			limit <<= 12;

		if (cache.flags.type.segc)
		{
			EXCEPTION(EXP_GP, mode == MODE_WRITE);
			EXCEPTION(EXP_GP, mode == MODE_READ && !cache.flags.type.code.r);
			EXCEPTION(EXP_GP, CPL > cache.flags.DPL && !(mode == MODE_EXEC && cache.flags.type.code.cnf));
		}
		else
		{
			EXCEPTION(EXP_GP, mode == MODE_EXEC);
			EXCEPTION(EXP_GP, mode == MODE_WRITE && !cache.flags.type.data.w);
			EXCEPTION(EXP_GP, CPL > cache.flags.DPL);

			if (cache.flags.type.data.exd)
				base -= limit;
		}
		EXCEPTION(EXP_GP, vaddr > limit);

		laddr = base + vaddr;
		DEBUG("base=0x%04x, limit=0x%02x, laddr=0x%02x", base, limit, laddr);
	}
	else
	{
		laddr = (sg.raw << 4) + vaddr;
		DEBUG("base=0x%04x, laddr=0x%02x", sg.raw << 4, laddr);
	}

	return laddr;
}

u32 get_phys(enum access_mode mode, u32 laddr)
{
	if (!is_ena_paging())
		return laddr;

	u32 paddr;
	u32 vpn;
	u16 offset;
	u8 cpl;
	struct pte pte;

	EXCEPTION(EXP_GP, !is_protected());

	cpl = get_segment(CS) & 3;
	vpn = laddr >> 12;
	offset = laddr & ((1 << 12) - 1);

	if (!search_tlb(vpn, &pte))
	{
		u32 pdir_base, ptbl_base;
		u16 pdir_index, ptbl_index;
		struct pde pde;

		pdir_index = laddr >> 22;
		ptbl_index = (laddr >> 12) & ((1 << 10) - 1);

		pdir_base = get_pdir_base() << 12;
		__read_data(&pde, pdir_base + pdir_index * sizeof(struct pde), sizeof(struct pde));
		EXCEPTION_WITH(EXP_PF, !pde.P, cpu.cr2.raw = laddr);
		EXCEPTION_WITH(EXP_PF, !pde.RW && mode == MODE_WRITE, cpu.cr2.raw = laddr);
		EXCEPTION_WITH(EXP_PF, !pde.US && cpl > 2, cpu.cr2.raw = laddr);

		ptbl_base = pde.ptbl_base << 12;
		__read_data(&pte, ptbl_base + ptbl_index * sizeof(struct pte), sizeof(struct pte));
		cache_tlb(vpn, pte);

		DEBUG("Cache TLB : pdir_base=0x%04x, ptbl_base=0x%04x {vpn=0x%04x, pfn=0x%04x}", pdir_base, ptbl_base, vpn, pte.page_base);
	}

	EXCEPTION_WITH(EXP_PF, !pte.P, cpu.cr2.raw = laddr);
	EXCEPTION_WITH(EXP_PF, !pte.RW && mode == MODE_WRITE, cpu.cr2.raw = laddr);
	EXCEPTION_WITH(EXP_PF, !pte.US && cpl > 2, cpu.cr2.raw = laddr);

	paddr = (pte.page_base << 12) + offset;

	return paddr;
}

u32 virt_to_phys(enum access_mode mode, enum sgreg seg, u32 vaddr)
{
	u32 laddr, paddr;

	laddr = get_linear(mode, seg, vaddr);
	paddr = get_phys(mode, laddr);

	if (!is_ena_a20gate())
		paddr &= (1 << 20) - 1;

	return paddr;
}

void push32(u32 value)
{
	u32 esp;

	update_gpreg32(ESP, -4);
	esp = get_gpreg32(ESP);
	write_mem32_seg(SS, esp, value);
}

u32 pop32(void)
{
	u32 esp, value;

	esp = get_gpreg16(ESP);
	value = read_mem32_seg(SS, esp);
	update_gpreg32(ESP, 4);

	return value;
}

void push16(u16 value)
{
	u16 sp;

	update_gpreg16(SP, -2);
	sp = get_gpreg16(SP);
	write_mem16_seg(SS, sp, value);
}

u16 pop16(void)
{
	u16 sp, value;

	sp = get_gpreg16(SP);
	value = read_mem16_seg(SS, sp);
	update_gpreg16(SP, 2);

	return value;
}