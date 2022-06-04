#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "common.h"
#include "memory.h"
#include "cpu.h"

static u8 memory[MEMORY_SIZE];

u8 get_code8(int index)
{
	return *(u8 *)(&memory[get_eip() + index]);
}

u16 get_code16(int index)
{
	return *(u16 *)(&memory[get_eip() + index]);
}

u32 get_code32(int index)
{
	return *(u32 *)(&memory[get_eip() + index]);
}

u32 trans_v2l(enum access_mode mode, enum sgreg seg, u32 vaddr)
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

u32 trans_v2p(enum access_mode mode, enum sgreg seg, u32 vaddr)
{
	u32 laddr, paddr;

	laddr = trans_v2l(mode, seg, vaddr);

	if (is_ena_paging())
	{
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
			read_data(&pde, pdir_base + pdir_index * sizeof(struct pde), sizeof(struct pde));
			EXCEPTION_WITH(EXP_PF, !pde.P, set_crn(2, laddr));
			EXCEPTION_WITH(EXP_PF, !pde.RW && mode == MODE_WRITE, set_crn(2, laddr));
			EXCEPTION_WITH(EXP_PF, !pde.US && cpl > 2, set_crn(2, laddr));

			ptbl_base = pde.ptbl_base << 12;
			read_data(&pte, ptbl_base + ptbl_index * sizeof(struct pte), sizeof(struct pte));
			cache_tlb(vpn, pte);

			INFO(3, "Cache TLB : pdir_base=0x%04x, ptbl_base=0x%04x {vpn=0x%04x, pfn=0x%04x}", pdir_base, ptbl_base, vpn, pte.page_base);
		}

		EXCEPTION_WITH(EXP_PF, !pte.P, set_crn(2, laddr));
		EXCEPTION_WITH(EXP_PF, !pte.RW && mode == MODE_WRITE, set_crn(2, laddr));
		EXCEPTION_WITH(EXP_PF, !pte.US && cpl > 2, set_crn(2, laddr));

		paddr = (pte.page_base << 12) + offset;
	}
	else
		paddr = laddr;

	if (!is_ena_a20gate())
		paddr &= (1 << 20) - 1;

	return paddr;
}

void write_mem8_seg(enum sgreg seg, u32 addr, u8 v)
{
	u32 paddr, io_base;

	paddr = trans_v2p(MODE_WRITE, seg, addr);
	if ((io_base = chk_memio(paddr)))
		write_memio8(io_base, paddr - io_base, v);
	else
		write_mem8(paddr, v);
}

void put_data8(enum sgreg seg, u32 addr, u8 v)
{
	write_mem8_seg(seg, addr, v);
}

int load_image(int fd, u64 addr)
{
	u8 buf[256];
	int len;

	while ((len = read(fd, buf, sizeof buf)) > 0)
	{
		int err = __mem_write(addr, buf, len);
		if (err)
			return err;
	}

	return 0;
}