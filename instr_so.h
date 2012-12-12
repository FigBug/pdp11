#ifndef _instr_so_h
#define _instr_so_h

void OP_clr(u16 addr);
void OP_clrb(u16 addr);
void OP_com(u16 addr);
void OP_comb(u16 addr);
void OP_inc(u16 addr);
void OP_incb(u16 addr);
void OP_dec(u16 addr);
void OP_decb(u16 addr);
void OP_neg(u16 addr);
void OP_negb(u16 addr);
void OP_tst(u16 addr);
void OP_tstb(u16 addr);
void OP_asr(u16 addr);
void OP_asrb(u16 addr);
void OP_asl(u16 addr);
void OP_aslb(u16 addr);
void OP_ror(u16 addr);
void OP_rorb(u16 addr);
void OP_rol(u16 addr);
void OP_rolb(u16 addr);
void OP_swab(u16 addr);
void OP_adc(u16 addr);
void OP_adcb(u16 addr);
void OP_sbc(u16 addr);
void OP_sbcb(u16 addr);
void OP_sxt(u16 addr);
void OP_mfps(u16 addr);
void OP_mtps(u16 addr);

#endif