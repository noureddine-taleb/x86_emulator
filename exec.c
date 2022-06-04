#include <stdlib.h>
#include <stdio.h>
#include "instruction.h"
#include "exec.h"
#include "common.h"
#include "cpu.h"
#include "memory.h"
#include "parser.h"

instruction_handler_t instructions[MAX_OPCODE];

bool exec(void)
{
	u16 opcode = OPCODE;

	if (opcode >> 8 == 0x0f)
		opcode = (opcode & 0xff) | 0x0100;

	if (opcode >= MAX_OPCODE || !instructions[opcode])
	{
		ERROR("not implemented OPCODE 0x%02x", OPCODE);
		return false;
	}

	instructions[opcode]();
	return true;
}

void add_rm8_r8(void)
{
	printf("add_rm8_r8\n");
}

void add_r8_rm8(void)
{
	printf("add_r8_rm8\n");
}

void add_al_imm8(void)
{
	printf("add_al_imm8\n");
}

void or_rm8_r8(void)
{
	printf("or_rm8_r8\n");
}

void or_r8_rm8(void)
{
	printf("or_r8_rm8\n");
}

void or_al_imm8(void)
{
	printf("or_al_imm8\n");
}

void and_rm8_r8(void)
{
	printf("and_rm8_r8\n");
}

void and_r8_rm8(void)
{
	printf("and_r8_rm8\n");
}

void and_al_imm8(void)
{
	printf("and_al_imm8\n");
}

void sub_rm8_r8(void)
{
	printf("sub_rm8_r8\n");
}

void sub_r8_rm8(void)
{
	printf("sub_r8_rm8\n");
}

void sub_al_imm8(void)
{
	printf("sub_al_imm8\n");
}

void xor_rm8_r8(void)
{
	printf("xor_rm8_r8\n");
}

void xor_r8_rm8(void)
{
	printf("xor_r8_rm8\n");
}

void xor_al_imm8(void)
{
	printf("xor_al_imm8\n");
}

void cmp_rm8_r8(void)
{
	printf("cmp_rm8_r8\n");
}

void cmp_r8_rm8(void)
{
	printf("cmp_r8_rm8\n");
}

void cmp_al_imm8(void)
{
	printf("cmp_al_imm8\n");
}

void jo_rel8(void)
{
	printf("jo_rel8\n");
}

void jno_rel8(void)
{
	printf("jno_rel8\n");
}

void jb_rel8(void)
{
	printf("jb_rel8\n");
}

void jnb_rel8(void)
{
	printf("jnb_rel8\n");
}

void jz_rel8(void)
{
	printf("jz_rel8\n");
}

void jnz_rel8(void)
{
	printf("jnz_rel8\n");
}

void jbe_rel8(void)
{
	printf("jbe_rel8\n");
}

void ja_rel8(void)
{
	printf("ja_rel8\n");
}

void js_rel8(void)
{
	printf("js_rel8\n");
}

void jns_rel8(void)
{
	printf("jns_rel8\n");
}

void jp_rel8(void)
{
	printf("jp_rel8\n");
}

void jnp_rel8(void)
{
	printf("jnp_rel8\n");
}

void jl_rel8(void)
{
	printf("jl_rel8\n");
}

void jnl_rel8(void)
{
	printf("jnl_rel8\n");
}

void jle_rel8(void)
{
	printf("jle_rel8\n");
}

void jnle_rel8(void)
{
	printf("jnle_rel8\n");
}

void test_rm8_r8(void)
{
	printf("test_rm8_r8\n");
}

void xchg_r8_rm8(void)
{
	printf("xchg_r8_rm8\n");
}

void mov_rm8_r8(void)
{
	u8 r8 = get_r8();
	set_rm8(r8);
}

void mov_rm16_r16(void)
{
	printf("mov_rm16_r16\n");
}

void mov_r8_rm8(void)
{
	printf("mov_r8_rm8\n");
}

void mov_sreg_rm16(void)
{
	printf("mov_sreg_rm16\n");
}

void nop(void)
{
	printf("nop\n");
}

void mov_al_moffs8(void)
{
	printf("mov_al_moffs8\n");
}

void mov_moffs8_al(void)
{
	printf("mov_moffs8_al\n");
}

void test_al_imm8(void)
{
	printf("test_al_imm8\n");
}

void mov_r8_imm8(void)
{
	printf("mov_r8_imm8\n");
}

void mov_rm8_imm8(void)
{
	printf("mov_rm8_imm8\n");
}

void retf(void)
{
	printf("retf\n");
}

void int3(void)
{
	printf("int3\n");
}

void int_imm8(void)
{
	printf("int_imm8\n");
}

void iret(void)
{
	printf("iret\n");
	exit(0);
}

void in_al_imm8(void)
{
	printf("in_al_imm8\n");
}

void out_imm8_al(void)
{
	printf("out_imm8_al\n");
}

void jmp(void)
{
	printf("jmp\n");
}

void in_al_dx(void)
{
	printf("in_al_dx\n");
}

void out_dx_al(void)
{
	printf("out_dx_al\n");
}

void cli(void)
{
	printf("cli\n");
}

void sti(void)
{
	printf("sti\n");
}

void cld(void)
{
	printf("cld\n");
}

void std(void)
{
	printf("std\n");
}

void hlt(void)
{
	printf("hlt\n");
}

void mov_r32_crn(void)
{
	printf("mov_r32_crn\n");
}

void mov_crn_r32(void)
{
	printf("mov_crn_r32\n");
}

void seto_rm8(void)
{
	printf("seto_rm8\n");
}

void setno_rm8(void)
{
	printf("setno_rm8\n");
}

void setb_rm8(void)
{
	printf("setb_rm8\n");
}

void setnb_rm8(void)
{
	printf("setnb_rm8\n");
}

void setz_rm8(void)
{
	printf("setz_rm8\n");
}

void setnz_rm8(void)
{
	printf("setnz_rm8\n");
}

void setbe_rm8(void)
{
	printf("setbe_rm8\n");
}

void seta_rm8(void)
{
	printf("seta_rm8\n");
}

void sets_rm8(void)
{
	printf("sets_rm8\n");
}

void setns_rm8(void)
{
	printf("setns_rm8\n");
}

void setp_rm8(void)
{
	printf("setp_rm8\n");
}

void setnp_rm8(void)
{
	printf("setnp_rm8\n");
}

void setl_rm8(void)
{
	printf("setl_rm8\n");
}

void setnl_rm8(void)
{
	printf("setnl_rm8\n");
}

void setle_rm8(void)
{
	printf("setle_rm8\n");
}

void setnle_rm8(void)
{
	printf("setnle_rm8\n");
}

void code_80(void)
{
	printf("code_80\n");
}

void code_82(void)
{
	printf("code_82\n");
}

void code_c0(void)
{
	printf("code_c0\n");
}

void code_f6(void)
{
	printf("code_f6\n");
}
