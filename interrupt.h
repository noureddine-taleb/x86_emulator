#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include "common.h"
#include "pic.h"

struct interrupt {
	u8 n;
	bool hard;
	struct interrupt *next;
};

union ivt {			// Real Mode
	u32 raw;
	struct {
		u16 offset;
		u16 segment;
	};
};

bool chk_irq(void);
void hundle_interrupt(void);

extern struct PIC pic_m;

extern struct interrupt *int_queue;

#endif // _INTERRUPT_H
