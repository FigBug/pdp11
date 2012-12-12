#ifndef _instr_do_h
#define _instr_do_h

void OP_bit(u16 src, u16 dst);
void OP_bitb(u16 src, u16 dst);
void OP_bic(u16 src, u16 dst);
void OP_bicb(u16 src, u16 dst);
void OP_bis(u16 src, u16 dst);
void OP_bisb(u16 src, u16 dst);
void OP_add(u16 src, u16 dst);
void OP_sub(u16 src, u16 dst);
void OP_mov(u16 src, u16 dst);
void OP_movb(u16 src, u16 dst);
void OP_cmp(u16 src, u16 dst);
void OP_cmpb(u16 src, u16 dst);
void OP_xor(u16 src, u16 dst);
void OP_mul(u16 src, u16 dst);
void OP_div(u16 src, u16 dst);

#endif