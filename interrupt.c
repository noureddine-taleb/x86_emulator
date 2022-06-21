#include "common.h"
#include "cpu.h"
#include "interrupt.h"
#include "exception.h"
#include "descriptor.h"
#include "memory.h"
#include "access.h"

struct interrupt *int_queue;

void queue_interrupt(u8 n, bool hard)
{
	struct interrupt *i, **last = &int_queue;

	i = malloc(sizeof(*i));
	i->n = n;
	i->hard = hard;

	while (*last)
		last = &(*last)->next;

	*last = i;
}

struct interrupt *pop_interrupt(void)
{
	struct interrupt *ret = int_queue;

	int_queue = int_queue->next;

	return ret;
}

bool chk_irq(void)
{
	u8 n_intr;

	if (!is_interrupt_enabled())
		return false;

	if (!chk_intreq())
		return false;

	n_intr = get_nintr(&pic_m);
	if (n_intr < 0)
		n_intr = get_nintr(&pic_s);
	queue_interrupt(n_intr, true);
	return true;
}

void save_regs(bool chpl, u16 cs)
{
	if (is_protected())
	{
		if (chpl)
		{
			u32 base, limit, esp;
			u16 ss;
			struct tss tss;

			base = get_dtreg_base(TR);
			limit = get_dtreg_limit(TR);
			EXCEPTION(EXP_TS, limit < sizeof(struct tss) - 1);

			__read_data(&tss, base, sizeof(struct tss));

			ss = get_segment(SS);
			esp = get_gpreg32(ESP);
			set_segment(SS, tss.ss0);
			set_gpreg32(ESP, tss.esp0);

			DEBUG("save_regs (ESP : 0x%08x->0x%08x, SS : 0x%04x->0x%04x)", esp, tss.esp0, ss, tss.ss0);
			push32(ss);
			push32(esp);
		}
		push32(get_eflags());
		push32(cs);
		push32(get_eip());
	}
	else
	{
		push16(get_flags());
		push16(cs);
		push16(get_ip());
	}
}

void hundle_interrupt(void)
{
	struct interrupt *intr;
	u16 cs;

	if (!int_queue)
		return;

	intr = pop_interrupt();

	if (is_protected())
	{
		struct int_gate_desc idt;
		u32 idt_base;
		u16 idt_limit, idt_offset;
		u8 CPL, RPL;

		CPL = get_segment(CS) & 3;

		idt_base = get_dtreg_base(IDTR);
		idt_limit = get_dtreg_limit(IDTR);
		idt_offset = intr->n << 3;

		EXCEPTION(EXP_GP, idt_offset > idt_limit);

		__read_data(&idt, idt_base + idt_offset, sizeof(struct int_gate_desc));
		RPL = ((struct sg_register *)&(idt.seg_sel))->RPL;

		DEBUG("int 0x%02x [CPL : %d, DPL : %d RPL : %d] (EIP : 0x%04x, CS : 0x%04x)", intr->n, CPL, idt.DPL, RPL, (idt.offset_h << 16) + idt.offset_l, idt.seg_sel);

		EXCEPTION(EXP_NP, !idt.P);
		EXCEPTION(EXP_GP, CPL < RPL);
		EXCEPTION(EXP_GP, !intr->hard && CPL > idt.DPL);

		cs = get_segment(CS);
		set_segment(CS, idt.seg_sel);
		save_regs(CPL ^ RPL, cs);
		set_eip((idt.offset_h << 16) + idt.offset_l);

		if (idt.type == TYPE_INTERRUPT)
			set_interrupt(false);
	}
	else
	{
		u32 idt_base;
		u16 idt_limit, idt_offset;
		union ivt ivt;

		idt_base = get_dtreg_base(IDTR);
		idt_limit = get_dtreg_limit(IDTR);
		idt_offset = intr->n << 2;

		EXCEPTION(EXP_GP, idt_offset > idt_limit);
		ivt.raw = read_mem32(idt_base + idt_offset);

		cs = get_segment(CS);
		set_segment(CS, ivt.segment);
		save_regs(false, cs);
		set_ip(ivt.offset);

		// set_interrupt(false);
		DEBUG("int 0x%02x (IP : 0x%04x, CS : 0x%04x)", intr->n, ivt.offset, ivt.segment);
	}
}
