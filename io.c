#include "common.h"
#include "io.h"
#include "memory.h"
#include "device.h"

struct io_mem_area io_mem_map[MAX_IO_AREAS];
struct io_port_area io_port_map[MAX_IO_AREAS];

/* port IO */
void set_portio(u16 addr, size_t len, struct device *dev)
{
	addr &= ~1;

	for (int i = 0; i < MAX_IO_AREAS; i++) {
		struct io_port_area *area = &io_port_map[i];
		if (!area->start && !area->end) {
			area->start = addr;
			area->end = addr + len;
			area->dev = dev;
			return;
		}
	}

	ERROR("io port areas are full\n");
}

struct io_port_area *get_portio_area(u16 addr)
{
	addr &= ~1;
	for (int i=0; i < MAX_IO_AREAS; i++) {
		struct io_port_area *area = &io_port_map[i];
		if (addr >= area->start && addr < area->end)
			return area;
	}

	return NULL;
}

u32 in_io32(u16 addr)
{
	u32 v = 0;

	for (int i = 0; i < 4; i++)
		v += in_io8(addr + i) << (8 * i);
	return v;
}

u16 in_io16(u16 addr)
{
	u16 v = 0;

	for (int i = 0; i < 2; i++)
		v += in_io8(addr + i) << (8 * i);
	return v;
}

u8 in_io8(u16 addr)
{
	u8 v = 0;
	struct io_port_area *area = get_portio_area(addr);
	if (area && area->dev->ops.in8)
		v = area->dev->ops.in8(addr);
	else
		ERROR("no device connected at port : 0x%04x", addr);
	return v;
}

void out_io32(u16 addr, u32 value)
{
	for (int i = 0; i < 4; i++)
		out_io8(addr + i, (value >> (8 * i)) & 0xff);
}

void out_io16(u16 addr, u16 value)
{
	for (int i = 0; i < 2; i++)
		out_io8(addr + i, (value >> (8 * i)) & 0xff);
}

void out_io8(u16 addr, u8 value)
{
	struct io_port_area *area = get_portio_area(addr);
	if (area && area->dev->ops.out8)
		area->dev->ops.out8(addr, value);
	else
		ERROR("no device connected at port : 0x%04x", addr);
}

/* memory mapped IO */
struct io_mem_area *get_memio_area(u32 addr)
{
	addr &= PAGE_MASK;
	for (int i=0; i < MAX_IO_AREAS; i++) {
		struct io_mem_area *area = &io_mem_map[i];
		if (addr >= area->start && addr < area->end)
			return area;
	}

	return NULL;
}

u32 chk_memio(u32 addr)
{
	struct io_mem_area *area = get_memio_area(addr);

	if (area)
		return area->start;
	return 0;
}

void set_memio(u32 base, size_t len, struct device *dev)
{
	u32 addr;

	ASSERT(!(base & PAGE_OFFSET));

	for (int i = 0; i < MAX_IO_AREAS; i++) {
		struct io_mem_area *area = &io_mem_map[i];
		if (!area->start && !area->end) {
			area->start = base;
			area->end = base + len;
			area->dev = dev;
			return;
		}
	}

	ERROR("memory map areas are full\n");
}

u32 read_memio32(u32 base, u32 offset)
{
	u32 v = 0;
	for(int i = 0; i < 4; i++)
		v += read_memio8(base, offset + i) << (8 * i);
	return v;
}

u16 read_memio16(u32 base, u32 offset)
{
	u16 v = 0;
	for(int i = 0; i < 2; i++)
		v += read_memio8(base, offset + i) << (8 * i);
	return v;
}

u8 read_memio8(u32 base, u32 offset)
{
	struct io_mem_area *area = get_memio_area(base);
	u8 v;
	if (area && area->dev->ops.read8)
		v = area->dev->ops.read8(offset);
	else
		ERROR("no device connected at port : 0x%04x", base);
	return v;
}

void write_memio32(u32 base, u32 offset, u8 value)
{
	for(int i=0; i<4; i++)
		write_memio8(base, offset + i, (value >> (8 * i)) & 0xff);
}

void write_memio16(u32 base, u32 offset, u8 value)
{
	for(int i = 0; i < 2; i++)
		write_memio8(base, offset + i, (value >> (8 * i)) & 0xff);
}

void write_memio8(u32 base, u32 offset, u8 value)
{
	struct io_mem_area *area = get_memio_area(offset);
	if (area && area->dev->ops.write8)
		area->dev->ops.write8(offset, value);
	else
		ERROR("no device connected at port : 0x%04x", base);
}
