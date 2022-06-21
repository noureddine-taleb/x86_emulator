#include "common.h"
#include "interrupt.h"
#include "pic.h"

struct PIC pic_m;
struct PIC pic_s;

void set_irq(struct PIC *pic, u8 n, struct IRQ *dev)
{
	if (n < MAX_IRQ)
		pic->irq[n] = dev;
	else
		ERROR("IRQ out of bound : %d", n);
}

bool chk_m2s_pic(struct PIC *pic, u8 n)
{
	return !pic->ic1.SNGL && !pic->pic_m && pic->ic3.raw & (1 << n);
} // this : master

s8 get_nintr(struct PIC *pic)
{
	u8 iva;
	int i;

	for (i = 0; i < MAX_IRQ && !((pic->irr >> i) & 1); i++)
		;
	if (i == MAX_IRQ)
		return -1;
	DEBUG("IRQ %d", !pic->pic_m ? i : i + MAX_IRQ);

	if (!pic->ic4.AEOI)
		pic->isr |= 1 << i;
	pic->irr ^= 1 << i;

	if (!pic->ic1.SNGL)
	{
		if (!pic->pic_m && pic->ic3.raw & (1 << i)) // master
			return -1;
		else if (pic->pic_m && !chk_m2s_pic(pic->pic_m, pic->ic3.ID)) // slave
			ERROR("");
	}

	iva = pic->ic4.PM ? pic->ic2.IVA_x86 << 3 : pic->ic1.IVA_l + (pic->ic2.IVA_h << 3);
	return iva + i;
}

bool irq_has_interrupt(struct IRQ *irq)
{
	if (irq->intr)
	{
		irq->intr = false;
		return true;
	}

	return false;
}

bool chk_intreq(void)
{
	int i;

	if (pic_m.init_icn)
		return false;

	for (i = 0; i < MAX_IRQ && !(pic_m.irq[i] && (pic_m.imr >> i) & 1 && irq_has_interrupt(pic_m.irq[i])); i++)
		;

	if (i == MAX_IRQ)
		return false;
	if (pic_m.isr && (1 << i) >= pic_m.isr)
		return false;

	pic_m.irr |= 1 << i;
	return true;
}
