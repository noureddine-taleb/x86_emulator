#ifndef _MEMORY_H
#define _MEMORY_H

#include "common.h"
#include "instruction.h"

#define MB (1 << 20)
#define MEMORY_SIZE (10 * MB)

#define READ_MEM32(addr) get_data32(select_segment(), addr)
#define READ_MEM16(addr) get_data16(select_segment(), addr)
#define READ_MEM8(addr) get_data8(select_segment(), addr)
#define WRITE_MEM32(addr, v) put_data32(select_segment(), addr, v)
#define WRITE_MEM16(addr, v) put_data16(select_segment(), addr, v)
#define WRITE_MEM8(addr, v) put_data8(select_segment(), addr, v)

enum access_mode {
	MODE_READ, 
	MODE_WRITE, 
	MODE_EXEC 
};

struct dt_register {
	u16 limit;
	u16 base_l;
	u16 base_h;
};

struct idt {
	u16 offset_l;
	u16 selector;
	u8 : 8;
	u8 type : 3;
	u8 D : 1;
	u8 : 1;
	u8 DPL : 2;
	u8 P : 1;
	u16 offset_h;
};

struct pde {
	u32 P : 1;
	u32 RW : 1;
	u32 US : 1;
	u32 PWT : 1;
	u32 PCD : 1;
	u32 A : 1;
	u32 : 1;
	u32 PS : 1;
	u32 G : 1;
	u32 : 3;
	u32 ptbl_base : 20;
};

struct pte {
	u32 P : 1;
	u32 RW : 1;
	u32 US : 1;
	u32 PWT : 1;
	u32 PCD : 1;
	u32 A : 1;
	u32 D : 1;
	u32 PAT : 1;
	u32 G : 1;
	u32 : 3;
	u32 page_base : 20;
};

u8 get_code8(int index);
u16 get_code16(int index);
u32 get_code32(int index);
int load_image(int fd, u64 addr);

#endif // _MEMORY_H
