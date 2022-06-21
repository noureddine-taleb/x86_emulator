#ifndef ACCESS
#define ACCESS

#include "common.h"
#include "cpu.h"
#include "memory.h"
#include "exception.h"

struct tlb_entry {
	u32 vpn;
	struct pte pte;
	bool full;
};

u32 get_linear(enum access_mode mode, enum sgreg seg, u32 vaddr);
u32 get_phys(enum access_mode mode, u32 laddr);
u32 virt_to_phys(enum access_mode mode, enum sgreg seg, u32 vaddr);

void push32(u32 value);
u32 pop32(void);
void push16(u16 value);
u16 pop16(void);

#endif // ACCESS
