#include "init.h"
#include "instruction.h"
#include "parser.h"

void set_instruction_handler(u16 opcode, instruction_handler_t func, u8 flags)
{
	if (opcode >> 8 == 0x0f)
		opcode = (opcode & 0xff) | 0x0100;
	ASSERT(opcode < MAX_OPCODE);

	instructions[opcode] = func;
	chk[opcode].flags = flags;
}

void init_instructions(void)
{
	set_instruction_handler(0x00, add_rm8_r8, CHK_MODRM);
	set_instruction_handler(0x02, add_r8_rm8, CHK_MODRM);
	set_instruction_handler(0x04, add_al_imm8, CHK_IMM8);
	set_instruction_handler(0x08, or_rm8_r8, CHK_MODRM);
	set_instruction_handler(0x0a, or_r8_rm8, CHK_MODRM);
	set_instruction_handler(0x0c, or_al_imm8, CHK_IMM8);
	set_instruction_handler(0x20, and_rm8_r8, CHK_MODRM);
	set_instruction_handler(0x22, and_r8_rm8, CHK_MODRM);
	set_instruction_handler(0x24, and_al_imm8, CHK_IMM8);
	set_instruction_handler(0x28, sub_rm8_r8, CHK_MODRM);
	set_instruction_handler(0x2a, sub_r8_rm8, CHK_MODRM);
	set_instruction_handler(0x2c, sub_al_imm8, CHK_IMM8);
	set_instruction_handler(0x30, xor_rm8_r8, CHK_MODRM);
	set_instruction_handler(0x32, xor_r8_rm8, CHK_MODRM);
	set_instruction_handler(0x34, xor_al_imm8, CHK_IMM8);
	set_instruction_handler(0x38, cmp_rm8_r8, CHK_MODRM);
	set_instruction_handler(0x3a, cmp_r8_rm8, CHK_MODRM);
	set_instruction_handler(0x3c, cmp_al_imm8, CHK_IMM8);
	set_instruction_handler(0x70, jo_rel8, CHK_IMM8);
	set_instruction_handler(0x71, jno_rel8, CHK_IMM8);
	set_instruction_handler(0x72, jb_rel8, CHK_IMM8);
	set_instruction_handler(0x73, jnb_rel8, CHK_IMM8);
	set_instruction_handler(0x74, jz_rel8, CHK_IMM8);
	set_instruction_handler(0x75, jnz_rel8, CHK_IMM8);
	set_instruction_handler(0x76, jbe_rel8, CHK_IMM8);
	set_instruction_handler(0x77, ja_rel8, CHK_IMM8);
	set_instruction_handler(0x78, js_rel8, CHK_IMM8);
	set_instruction_handler(0x79, jns_rel8, CHK_IMM8);
	set_instruction_handler(0x7a, jp_rel8, CHK_IMM8);
	set_instruction_handler(0x7b, jnp_rel8, CHK_IMM8);
	set_instruction_handler(0x7c, jl_rel8, CHK_IMM8);
	set_instruction_handler(0x7d, jnl_rel8, CHK_IMM8);
	set_instruction_handler(0x7e, jle_rel8, CHK_IMM8);
	set_instruction_handler(0x7f, jnle_rel8, CHK_IMM8);
	set_instruction_handler(0x84, test_rm8_r8, CHK_MODRM);
	set_instruction_handler(0x86, xchg_r8_rm8, CHK_MODRM);
	set_instruction_handler(0x88, mov_rm8_r8, CHK_MODRM);
	set_instruction_handler(0x89, mov_rm16_r16, CHK_MODRM);
	set_instruction_handler(0x8a, mov_r8_rm8, CHK_MODRM);
	set_instruction_handler(0x8e, mov_sreg_rm16, CHK_MODRM);
	set_instruction_handler(0x90, nop, 0);
	set_instruction_handler(0xa0, mov_al_moffs8, CHK_MOFFS);
	set_instruction_handler(0xa2, mov_moffs8_al, CHK_MOFFS);
	set_instruction_handler(0xa8, test_al_imm8, CHK_IMM8);
	for (int i = 0; i < 8; i++)
		set_instruction_handler(0xb0 + i, mov_r8_imm8, CHK_IMM8);
	set_instruction_handler(0xc6, mov_rm8_imm8, CHK_MODRM | CHK_IMM8);
	set_instruction_handler(0xcb, retf, 0);
	set_instruction_handler(0xcc, int3, 0);
	set_instruction_handler(0xcd, int_imm8, CHK_IMM8);
	set_instruction_handler(0xcf, iret, 0);
	set_instruction_handler(0xe4, in_al_imm8, CHK_IMM8);
	set_instruction_handler(0xe6, out_imm8_al, CHK_IMM8);
	set_instruction_handler(0xeb, jmp, CHK_IMM8);
	set_instruction_handler(0xec, in_al_dx, 0);
	set_instruction_handler(0xee, out_dx_al, 0);
	set_instruction_handler(0xfa, cli, 0);
	set_instruction_handler(0xfb, sti, 0);
	set_instruction_handler(0xfc, cld, 0);
	set_instruction_handler(0xfd, std, 0);
	set_instruction_handler(0xf4, hlt, 0);

	set_instruction_handler(0x0f20, mov_r32_crn, CHK_MODRM);
	set_instruction_handler(0x0f22, mov_crn_r32, CHK_MODRM);
	set_instruction_handler(0x0f90, seto_rm8, CHK_MODRM);
	set_instruction_handler(0x0f91, setno_rm8, CHK_MODRM);
	set_instruction_handler(0x0f92, setb_rm8, CHK_MODRM);
	set_instruction_handler(0x0f93, setnb_rm8, CHK_MODRM);
	set_instruction_handler(0x0f94, setz_rm8, CHK_MODRM);
	set_instruction_handler(0x0f95, setnz_rm8, CHK_MODRM);
	set_instruction_handler(0x0f96, setbe_rm8, CHK_MODRM);
	set_instruction_handler(0x0f97, seta_rm8, CHK_MODRM);
	set_instruction_handler(0x0f98, sets_rm8, CHK_MODRM);
	set_instruction_handler(0x0f99, setns_rm8, CHK_MODRM);
	set_instruction_handler(0x0f9a, setp_rm8, CHK_MODRM);
	set_instruction_handler(0x0f9b, setnp_rm8, CHK_MODRM);
	set_instruction_handler(0x0f9c, setl_rm8, CHK_MODRM);
	set_instruction_handler(0x0f9d, setnl_rm8, CHK_MODRM);
	set_instruction_handler(0x0f9e, setle_rm8, CHK_MODRM);
	set_instruction_handler(0x0f9f, setnle_rm8, CHK_MODRM);

	set_instruction_handler(0x80, code_80, CHK_MODRM | CHK_IMM8);
	set_instruction_handler(0x82, code_82, CHK_MODRM | CHK_IMM8);
	set_instruction_handler(0xc0, code_c0, CHK_MODRM | CHK_IMM8);
	set_instruction_handler(0xf6, code_f6, CHK_MODRM);
}
