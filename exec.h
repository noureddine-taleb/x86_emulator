#ifndef _EXEC_H
#define _EXEC_H

#include "common.h"
#include "instruction.h"
#include "cpu.h"

typedef void (*instruction_handler_t)(void);

extern instruction_handler_t instructions[MAX_OPCODE];

bool exec(void);

void add_rm8_r8(void);
void add_r8_rm8(void);
void add_al_imm8(void);
void or_rm8_r8(void);
void or_r8_rm8(void);
void or_al_imm8(void);
void and_rm8_r8(void);
void and_r8_rm8(void);
void and_al_imm8(void);
void sub_rm8_r8(void);
void mov_rm16_r16(void);
void sub_r8_rm8(void);
void sub_al_imm8(void);
void xor_rm8_r8(void);
void xor_r8_rm8(void);
void xor_al_imm8(void);
void cmp_rm8_r8(void);
void cmp_r8_rm8(void);
void cmp_al_imm8(void);
void jo_rel8(void);
void jno_rel8(void);
void jb_rel8(void);
void jnb_rel8(void);
void jz_rel8(void);
void jnz_rel8(void);
void jbe_rel8(void);
void ja_rel8(void);
void js_rel8(void);
void jns_rel8(void);
void jp_rel8(void);
void jnp_rel8(void);
void jl_rel8(void);
void jnl_rel8(void);
void jle_rel8(void);
void jnle_rel8(void);
void test_rm8_r8(void);
void xchg_r8_rm8(void);
void mov_rm8_r8(void);
void mov_r8_rm8(void);
void mov_sreg_rm16(void);
void nop(void);
void mov_al_moffs8(void);
void mov_moffs8_al(void);
void test_al_imm8(void);
void mov_r8_imm8(void);
void mov_rm8_imm8(void);
void retf(void);
void int3(void);
void int_imm8(void);
void iret(void);
void in_al_imm8(void);
void out_imm8_al(void);
void jmp(void);
void in_al_dx(void);
void out_dx_al(void);
void cli(void);
void sti(void);
void cld(void);
void std(void);
void hlt(void);
void mov_r32_crn(void);
void mov_crn_r32(void);
void seto_rm8(void);
void setno_rm8(void);
void setb_rm8(void);
void setnb_rm8(void);
void setz_rm8(void);
void setnz_rm8(void);
void setbe_rm8(void);
void seta_rm8(void);
void sets_rm8(void);
void setns_rm8(void);
void setp_rm8(void);
void setnp_rm8(void);
void setl_rm8(void);
void setnl_rm8(void);
void setle_rm8(void);
void setnle_rm8(void);
void code_80(void);
void code_82(void);
void code_c0(void);
void code_f6(void);

#endif // _EXEC_H
