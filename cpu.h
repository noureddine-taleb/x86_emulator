#ifndef _CPU_H
#define _CPU_H

#include "common.h"

//#define get_eip() get_eip()
//#define GET_IP() get_ip()
//#define SET_EIP(v) set_eip(v)
//#define SET_IP(v) set_ip(v)
// #define update_eip(v) update_eip(v)
//#define UPDATE_IP(v) update_ip(v)
//#define GET_GPREG(reg) get_gpreg(reg)
//#define SET_GPREG(reg, v) set_gpreg(reg, v)
//#define UPDATE_GPREG(reg, v) update_gpreg(reg, v)
//#define EFLAGS_UPDATE_ADD(v1, v2) update_eflags_add(v1, v2)
//#define EFLAGS_UPDATE_OR(v1, v2) update_eflags_or(v1, v2)
//#define EFLAGS_UPDATE_AND(v1, v2) update_eflags_and(v1, v2)
//#define EFLAGS_UPDATE_SUB(v1, v2) update_eflags_sub(v1, v2)
//#define EFLAGS_UPDATE_MUL(v1, v2) update_eflags_mul(v1, v2)
//#define EFLAGS_UPDATE_IMUL(v1, v2) update_eflags_imul(v1, v2)
//#define EFLAGS_UPDATE_SHL(v1, v2) update_eflags_shl(v1, v2)
//#define EFLAGS_UPDATE_SHR(v1, v2) update_eflags_shr(v1, v2)
//#define EFLAGS_CF is_carry()
//#define EFLAGS_PF is_parity()
//#define EFLAGS_ZF is_zero()
//#define EFLAGS_SF is_sign()
//#define EFLAGS_OF is_overflow()
//#define EFLAGS_DF is_direction()
//#define PUSH32(v) push32(v)
//#define PUSH16(v) push16(v)
//#define POP32() pop32()
//#define POP16() pop16()

enum reg32
{
	EAX,
	ECX,
	EDX,
	EBX,
	ESP,
	EBP,
	ESI,
	EDI,
	GPREGS_COUNT
};

enum reg16
{
	AX,
	CX,
	DX,
	BX,
	SP,
	BP,
	SI,
	DI
};

enum reg8
{
	AL,
	CL,
	DL,
	BL,
	AH,
	CH,
	DH,
	BH
};

enum sgreg
{
	ES,
	CS,
	SS,
	DS,
	FS,
	GS,
	SGREGS_COUNT
};

enum dtreg
{
	GDTR,
	IDTR,
	LDTR,
	TR,
	DTREGS_COUNT
};

union gp_register
{
	u32 reg32;
	u16 reg16;
	struct
	{
		u8 reg8_l;
		u8 reg8_h;
	};
};

struct sg_register_cache
{
	u32 base;
	u32 limit : 20;

	union
	{
		u16 raw : 12;

		union
		{
			struct
			{
				u8 : 1;
				u8 w : 1; // 0:r, 1:rw
				u8 exd : 1;
				u8 : 1; // = 0
			} data;
			struct
			{
				u8 : 1;
				u8 r : 1; // 0:x, 1;xr
				u8 cnf : 1;
				u8 : 1; // = 1
			} code;
			struct
			{
				u8 A : 1;
				u8 : 2;
				u8 segc : 1;
			};
		} type;

		struct
		{
			u8 : 4;
			u8 S : 1;
			u8 DPL : 2;
			u8 P : 1;

			u8 AVL : 1;
			u8 : 1;
			u8 DB : 1;
			u8 G : 1;
		};
	} flags;
};

struct sg_register
{
	union
	{
		u16 raw;
		struct
		{
			u16 RPL : 2;
			u16 TI : 1;
			u16 index : 13;
		};
	};

	struct sg_register_cache cache;
};

struct dt_register
{
	u16 selector; // LDTR, TR
	u32 base;
	u16 limit;
};

struct x86_cpu
{
	union
	{
		u32 eip;
		u16 ip;
	};

	union gp_register gpregs[GPREGS_COUNT];
	struct sg_register sgregs[SGREGS_COUNT];
	struct dt_register dtregs[DTREGS_COUNT];

	union
	{
		u32 reg32;
		u16 reg16;

		struct
		{
			u32 CF : 1;
			u32 : 1; // 1
			u32 PF : 1;
			u32 : 1; // 0
			u32 AF : 1;
			u32 : 1; // 0
			u32 ZF : 1;
			u32 SF : 1;
			u32 TF : 1;
			u32 IF : 1;
			u32 DF : 1;
			u32 OF : 1;
			u32 IOPL : 2;
			u32 NT : 1;
			u32 : 1; // 0
			u32 RF : 1;
			u32 VM : 1;
			u32 AC : 1;
			u32 VIF : 1;
			u32 VIP : 1;
			u32 ID : 1;
		};
	} eflags;

	union
	{
		u32 raw;
		struct
		{
			u32 PE : 1;
			u32 MP : 1;
			u32 EM : 1;
			u32 TS : 1;
			u32 ET : 1;
			u32 NE : 1;
			u32 : 10;
			u32 WP : 1;
			u32 : 1;
			u32 AM : 1;
			u32 : 10;
			u32 NW : 1;
			u32 CD : 1;
			u32 PG : 1;
		};
	} cr0;

	union
	{
		u32 raw;
	} cr1;

	union
	{
		u32 raw;
	} cr2;

	union
	{
		u32 raw;
		struct
		{
			u32 : 3;
			u32 PWT : 1;
			u32 PCD : 1;
			u32 : 7;
			u32 PageDirBase : 20;
		};
	} cr3;

	union
	{
		u32 raw;
		struct
		{
			u32 VME : 1;
			u32 PVI : 1;
			u32 TSD : 1;
			u32 DE : 1;
			u32 PSE : 1;
			u32 PAE : 1;
			u32 MCE : 1;
			u32 PGE : 1;
			u32 PCE : 1;
			u32 OSFXSR : 1;
			u32 OSXMMEXCPT : 1;
			u32 : 21;
		};
	} cr4;

	bool __halt;
};

u32 get_eip(void);
u32 get_ip(void);
u32 get_gpreg32(enum reg32 n);
u16 get_gpreg16(enum reg16 n);
u8 get_gpreg8(enum reg8 n);
void get_sgreg(enum sgreg n, struct sg_register *reg);
u32 get_dtreg_selector(enum dtreg n);
u32 get_dtreg_base(enum dtreg n);
u16 get_dtreg_limit(enum dtreg n);
void set_eip(u32 v);
void set_ip(u16 v);
void set_gpreg32(enum reg32 n, u32 v);
void set_gpreg16(enum reg16 n, u16 v);
void set_gpreg8(enum reg8 n, u8 v);
void set_sgreg(enum sgreg n, struct sg_register *reg);
void set_dtreg(enum dtreg n, u16 sel, u32 base, u16 limit);
u32 update_eip(s32 v);
u32 update_ip(s32 v);
u32 update_gpreg32(enum reg32 n, s32 v);
u16 update_gpreg16(enum reg16 n, s16 v);
u8 update_gpreg8(enum reg8 n, s8 v);
bool is_halt(void);
void do_halt(bool h);
bool is_mode32(void);
bool is_protected(void);
bool is_ena_paging(void);
u32 get_pdir_base(void);
u16 get_segment(enum sgreg reg);
void set_segment(enum sgreg reg, u16 sel);
void do_halt(bool h);
bool is_halt(void);
bool is_interrupt_enabled(void);
u32 get_eflags(void);
void set_eflags(u32 v);
u16 get_flags(void);
void set_flags(u16 v);
void set_interrupt(bool interrupt);

extern struct x86_cpu cpu;

#endif // _CPU_H
