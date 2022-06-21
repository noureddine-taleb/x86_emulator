#ifndef _MEMORY_H
#define _MEMORY_H

#include "common.h"
#include "instruction.h"

#define MB (1 << 20)
#define MEMORY_SIZE (10 * MB)
#define IN_RANGE(addr, len)	(addr+len-1 < MEMORY_SIZE)

#define PAGE_SIZE (1 << 12)
#define PAGE_OFFSET (PAGE_SIZE - 1)
#define PAGE_MASK (~(PAGE_OFFSET))

#define READ_MEM32(addr) get_data32(select_segment(), addr)
#define READ_MEM16(addr) get_data16(select_segment(), addr)
#define READ_MEM8(addr) get_data8(select_segment(), addr)
#define WRITE_MEM32(addr, v) put_data32(select_segment(), addr, v)
#define WRITE_MEM16(addr, v) put_data16(select_segment(), addr, v)
#define WRITE_MEM8(addr, v) put_data8(select_segment(), addr, v)

enum access_mode
{
	MODE_READ,
	MODE_WRITE,
	MODE_EXEC
};

struct idt
{
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

struct pde
{
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

struct pte
{
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

void put_data16(enum sgreg seg, u32 addr, u16 v);
void put_data32(enum sgreg seg, u32 addr, u32 v);
void put_data8(enum sgreg seg, u32 addr, u8 v);

void write_mem32_seg(enum sgreg seg, u32 addr, u32 v);
void write_mem16_seg(enum sgreg seg, u32 addr, u16 v);
void write_mem8_seg(enum sgreg seg, u32 addr, u8 v);

u32 read_mem32(u32 addr);
u16 read_mem16(u32 addr);
u8 read_mem8(u32 addr);

void write_mem32(u32 addr, u32 v);
void write_mem16(u32 addr, u16 v);
void write_mem8(u32 addr, u8 v);


u32 read_mem32_seg(enum sgreg seg, u32 addr);
u16 read_mem16_seg(enum sgreg seg, u32 addr);
u8 read_mem8_seg(enum sgreg seg, u32 addr);

void __read_data(void *dst, u32 src_addr, size_t size);
void __write_data(u32 dst_addr, char *buf, size_t size);

int load_image(int fd, u64 addr);

bool is_ena_a20gate(void);
void set_a20gate(bool ena);

extern bool a20gate;

#endif // _MEMORY_H
