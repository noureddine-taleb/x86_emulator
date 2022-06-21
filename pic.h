#ifndef _PIC_H
#define _PIC_H

#include "interrupt.h"

#define MAX_IRQ 8

struct IRQ {
	bool intr;
};

struct PIC {
	struct PIC *pic_m;
	struct IRQ *irq[MAX_IRQ];
	u8 irr;
	u8 isr;
	u8 imr;

	union {
		u8 raw;
		struct {
			u8 IC4 : 1;
			u8 SNGL : 1;
			u8 ADI : 1;
			u8 LTIM : 1;
			u8 : 1;
			u8 IVA_l : 3;	// MCS-80/85
		};
	} ic1;

	union {
		u8 raw;
		struct {
			u8 IVA_h : 3;	// MCS-80/85
			u8 IVA_x86 : 5;
		};
	} ic2;

	union {
		u8 raw;
		struct {
			u8 S0 : 1;
			u8 S1 : 1;
			u8 S2 : 1;
			u8 S3 : 1;
			u8 S4 : 1;
			u8 S5 : 1;
			u8 S6 : 1;
			u8 S7 : 1;
		};

		struct {
			u8 ID : 3;
		};
	} ic3;

	union {
		u8 raw;
		struct {
			u8 PM : 1;
			u8 AEOI : 1;
			u8 MS : 1;
			u8 BUF : 1;
			u8 SFNM : 1;
		};
	} ic4;

	s8 init_icn;
};

void set_irq(struct PIC *pic, u8 n, struct IRQ *dev);
s8 get_nintr(struct PIC *pic);
bool chk_intreq(void);
bool chk_m2s_pic(struct PIC *pic, u8 n);

extern struct PIC pic_m;
extern struct PIC pic_s;

#endif // _PIC_H
