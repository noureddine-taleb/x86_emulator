#ifndef _MEMORY_H
#define _MEMORY_H

#include "common.h"

#define MB (1 << 20)
#define MEMORY_SIZE (10 * MB)

u8 get_code8(int index);
u16 get_code16(int index);
u32 get_code32(int index);
int load_image(int fd, u64 addr);

#endif // _MEMORY_H
