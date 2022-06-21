#ifndef _IO_H
#define _IO_H

#include "device.h"

#define MAX_IO_AREAS 255

struct io_mem_area {
	u32 start;
	u32 end;

	struct device *dev;
};

struct io_port_area {
	u32 start;
	u32 end;

	struct device *dev;
};

void set_portio(u16 addr, size_t len, struct device *dev);
struct io_port_area *get_portio_area(u16 addr);
u32 in_io32(u16 addr);
u16 in_io16(u16 addr);
u8 in_io8(u16 addr);
void out_io32(u16 addr, u32 value);
void out_io16(u16 addr, u16 value);
void out_io8(u16 addr, u8 value);
struct io_mem_area *get_memio_area(u32 addr);
u32 chk_memio(u32 addr);
void set_memio(u32 base, size_t len, struct device *dev);
u32 read_memio32(u32 base, u32 offset) ;
u16 read_memio16(u32 base, u32 offset);
u8 read_memio8(u32 base, u32 offset);
void write_memio32(u32 base, u32 offset, u8 value);
void write_memio16(u32 base, u32 offset, u8 value);
void write_memio8(u32 base, u32 offset, u8 value);

#endif // _IO_H
