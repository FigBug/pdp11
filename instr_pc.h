#ifndef _instr_pc_h
#define _instr_pc_h

void OP_br(u8 off);
void OP_bne(u8 off);
void OP_beq(u8 off);
void OP_bpl(u8 off);
void OP_bmi(u8 off);
void OP_bvc(u8 off);
void OP_bvs(u8 off);
void OP_bcc(u8 off);
void OP_bcs(u8 off);
void OP_bge(u8 off);
void OP_blt(u8 off);
void OP_bgt(u8 off);
void OP_ble(u8 off);
void OP_bhi(u8 off);
void OP_blos(u8 off);
void OP_bhis(u8 off);

#endif