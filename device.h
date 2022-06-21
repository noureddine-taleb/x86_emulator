#ifndef _DEVICE_H
#define _DEVICE_H

#include "common.h"

struct device_ops
{
	u8 (*in8)(u16 addr);
	void (*out8)(u16 addr, u8 v);
	void (*write8)(u16 off, u8 v);
	u8 (*read8)(u16 off);
};

struct device
{
	u32 iomem;
	u32 size;
	struct device_ops ops;
};

#endif // _DEVICE_H
