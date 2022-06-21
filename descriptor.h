#ifndef _DESCRIPTOR_H
#define _DESCRIPTOR_H

#include "common.h"

#define TYPE_TSS	0x01
#define TYPE_LDT	0x02
#define TYPE_CALL	0x04
#define TYPE_TASK	0x05
#define TYPE_INTERRUPT	0x06
#define TYPE_TRAP	0x07

#define TYPE_DATA	0x10
#define TYPE_CODE	0x18

struct descriptor {
	u16 limit_l;
	u16 base_l;
	u8 base_m;
	u8 type : 3;
	u8 D : 1;
	u8 S : 1;
	u8 DPL : 2;
	u8 P : 1;
	u8 limit_h : 4;
	u8 AVL : 1;
	u8 : 2;
	u8 G : 1;
	u8 base_h;
};

// Segment Descriptor
struct seg_desc {
	u16 limit_l;
	u16 base_l;
	u8 base_m;
	union {
		union {
			struct {
				u8 : 1;
				u8 w : 1;		// 0:r, 1:rw
				u8 exd: 1;
				u8 : 5;
			} data;
			struct {
				u8 : 1;
				u8 r : 1;		// 0:x, 1;xr
				u8 cnf : 1;
				u8 : 5;
			} code;
			struct{
				u8 A : 1;
				u8 : 2;
				u8 segc : 1;
				u8 : 4;
			};
		} type;

		struct {
			u8 : 4;
			u8 S : 1;			// 1
			u8 DPL : 2;
			u8 P : 1;
		};
	};
	u8 limit_h : 4;
	u8 AVL : 1;
	u8 : 1;
	u8 DB : 1;
	u8 G : 1;
	u8 base_h;
};

// System Segment Descriptor
struct tss_desc {
	u16 limit_l;
	u16 base_l;
	u8 base_m;
	union {
		struct {
			u8 : 1;
			u8 B : 1;
		};

		struct {
			u8 type : 3;	// 1 or 3
			u8 D : 1;		// 0:16bit, 1:32bit
			u8 S : 1;		// 0
			u8 DPL : 2;
			u8 P : 1;
		};
	};
	u8 limit_h : 4;
	u8 AVL : 1;
	u8 : 2;
	u8 G : 1;
	u8 base_h;
};

struct ldt_desc {
	u16 limit_l;
	u16 base_l;
	u8 base_m;
	u8 type : 3;	// 2
	u8 D : 1;
	u8 S : 1;		// 0
	u8 DPL : 2;
	u8 P : 1;
	u8 limit_h : 4;
	u8 AVL : 1;
	u8 : 2;
	u8 G : 1;
	u8 base_h;
};

// Gate Descriptor
struct call_gate_desc {
	u16 offset_l;
	u16 seg_sel;
	u8 pc : 5;
	u8 : 3;
	u8 type : 3;	// 4
	u8 D : 1;
	u8 S : 1;		// 0
	u8 DPL : 2;
	u8 P : 1;
	u16 offset_h;
};

struct task_gate_desc {
	u16 : 16;
	u16 tss_sel;
	u8 : 8;
	u8 type : 3;	// 5
	u8 : 1;
	u8 S : 1;		// 0
	u8 DPL : 2;
	u8 P : 1;
	u16 : 16;
};

struct int_gate_desc {
	u16 offset_l;
	u16 seg_sel;
	u8 : 8;
	u8 type : 3;	// 6
	u8 D : 1;
	u8 S : 1;		// 0
	u8 DPL : 2;
	u8 P : 1;
	u16 offset_h;
};

struct trap_gate_desc {
	u16 offset_l;
	u16 seg_sel;
	u8 : 8;
	u8 type : 3;	// 7
	u8 D : 1;
	u8 S : 1;		// 0
	u8 DPL : 2;
	u8 P : 1;
	u16 offset_h;
};

struct tss {
	u16 prev_sel;
	u16 : 16;
	u32 esp0;
	u16 ss0;
	u16 : 16;
	u32 esp1;
	u16 ss1;
	u16 : 16;
	u32 esp2;
	u16 ss2;
	u16 : 16;
	u32 cr3;
	u32 eip;
	u32 eflags;
	u32 eax;
	u32 ecx;
	u32 edx;
	u32 ebx;
	u32 esp;
	u32 ebp;
	u32 esi;
	u32 edi;
	u16 es;
	u16 : 16;
	u16 cs;
	u16 : 16;
	u16 ss;
	u16 : 16;
	u16 ds;
	u16 : 16;
	u16 fs;
	u16 : 16;
	u16 gs;
	u16 : 16;
	u16 ldtr;
	u16 : 16;
	u16 T : 1;
	u16 : 15;
	u16 io_base;
};

#endif
