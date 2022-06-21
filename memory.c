#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "common.h"
#include "memory.h"
#include "cpu.h"
#include "access.h"
#include "io.h"

static u8 memory[MEMORY_SIZE];
bool a20gate;

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

u32 read_mem32_seg(enum sgreg seg, u32 addr)
{
	u32 paddr, io_base;

	paddr = virt_to_phys(MODE_READ, seg, addr);
	return (io_base = chk_memio(paddr)) ? read_memio32(io_base, paddr - io_base) : read_mem32(paddr);
}

u16 read_mem16_seg(enum sgreg seg, u32 addr)
{
	u32 paddr, io_base;

	paddr = virt_to_phys(MODE_READ, seg, addr);
	return (io_base = chk_memio(paddr)) ? read_memio16(io_base, paddr - io_base) : read_mem16(paddr);
}

u8 read_mem8_seg(enum sgreg seg, u32 addr)
{
	u32 paddr, io_base;

	paddr = virt_to_phys(MODE_READ, seg, addr);
	return (io_base = chk_memio(paddr)) ? read_memio8(io_base, paddr - io_base) : read_mem8(paddr);
}

void write_mem32_seg(enum sgreg seg, u32 addr, u32 v)
{
	u32 paddr, io_base;

	paddr = virt_to_phys(MODE_WRITE, seg, addr);
	if ((io_base = chk_memio(paddr)))
		write_memio32(io_base, paddr - io_base, v);
	else
		write_mem32(paddr, v);
}

void write_mem16_seg(enum sgreg seg, u32 addr, u16 v)
{
	u32 paddr, io_base;

	paddr = virt_to_phys(MODE_WRITE, seg, addr);
	if ((io_base = chk_memio(paddr)))
		write_memio16(io_base, paddr - io_base, v);
	else
		write_mem16(paddr, v);
}

void write_mem8_seg(enum sgreg seg, u32 addr, u8 v)
{
	u32 paddr, io_base;

	paddr = virt_to_phys(MODE_WRITE, seg, addr);
	if ((io_base = chk_memio(paddr)))
		write_memio8(io_base, paddr - io_base, v);
	else
		write_mem8(paddr, v);
}

void put_data16(enum sgreg seg, u32 addr, u16 v)
{
	write_mem16_seg(seg, addr, v);
}

void put_data32(enum sgreg seg, u32 addr, u32 v)
{
	write_mem32_seg(seg, addr, v);
}

void put_data8(enum sgreg seg, u32 addr, u8 v)
{
	write_mem8_seg(seg, addr, v);
}

void __read_data(void *dst, u32 src_addr, size_t size)
{
	memcpy(dst, &memory[src_addr], size);
}

void __write_data(u32 dst_addr, char *buf, size_t size)
{
	memcpy(&memory[dst_addr], buf, size);
}

int load_image(int fd, u64 addr)
{
	u8 buf[256];
	int len;

	while ((len = read(fd, buf, sizeof buf)) > 0)
	{
		__write_data(addr, buf, len);
		addr += len;
	}

	return 0;
}

u32 read_mem32(u32 addr)
{
	return IN_RANGE(addr, 4) ? *((u32*)&memory[addr]) : 0;
}

u16 read_mem16(u32 addr)
{
	return IN_RANGE(addr, 2) ? *((u16*)&memory[addr]) : 0;
}

u8 read_mem8(u32 addr)
{
	return IN_RANGE(addr, 1) ? memory[addr] : 0;
}


void write_mem32(u32 addr, u32 v)
{
	if(IN_RANGE(addr, 4)) *((u32*)&memory[addr]) = v;
}

void write_mem16(u32 addr, u16 v)
{
	if(IN_RANGE(addr, 2)) *((u16*)&memory[addr]) = v;
}

void write_mem8(u32 addr, u8 v)
{
	if(IN_RANGE(addr, 1)) memory[addr] = v;
}

bool is_ena_a20gate(void)
{
	return a20gate;
}

void set_a20gate(bool ena)
{
	a20gate = ena;
}