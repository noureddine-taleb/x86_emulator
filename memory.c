#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "common.h"
#include "memory.h"
#include "cpu.h"

static u8 memory[MEMORY_SIZE];

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

int __mem_write(u64 addr, u64 count, u8 *buf)
{
	if (addr >= MEMORY_SIZE || addr + count > MEMORY_SIZE)
	{
		return errno = -E2BIG;
	}

	memcpy(memory + addr, buf, count);
	return 0;
}

int load_image(int fd, u64 addr)
{
	u8 buf[256];
	int len;

	while ((len = read(fd, buf, sizeof buf)) > 0)
	{
		int err = __mem_write(addr, len, buf);
		if (err)
			return err;
	}

	return 0;
}