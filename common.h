#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>

#define true 1
#define false 0

#define DEBUG(fmt, ...) printf("DEBUG:" fmt "\n", ##__VA_ARGS__)
#define WARN(fmt, ...) printf("WARN:" fmt "\n", ##__VA_ARGS__)
#define ERROR(fmt, ...) printf("ERROR:" fmt "\n", ##__VA_ARGS__)

#define _STRINGIFY(s) #s
#define STRINGIFY(s) _STRINGIFY(s)

#define ASSERT(cond)                                          \
	if (!(cond))                                              \
	{                                                         \
		die("ASSERT FAILED "                                  \
			"(" __FILE__ ":" STRINGIFY(__LINE__) "):" #cond); \
	}
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

typedef u8 bool;

void die(char *fmt, ...) __attribute__((__noreturn__)) __attribute__((format(printf, 1, 2)));

#endif // _COMMON_H
