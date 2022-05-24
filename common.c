#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void die(char *fmt, ...)
{
	va_list varg;

	va_start(varg, fmt);
	vfprintf(stderr, fmt, varg);
	va_end(varg);
	exit(1);
}
