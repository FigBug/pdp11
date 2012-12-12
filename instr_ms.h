#ifndef _instr_ms_h
#define _instr_ms_h

void OP_jmp(u16 addr);
void OP_jsr(u16 rr, u16 dd);
void OP_rts(u16 addr);
void OP_mark(u16 nn);
void OP_sob(u16 rr, u16 offset);

void OP_emt(void);
void OP_trap(void);
void OP_bpt(void);
void OP_iot(void);
void OP_rti(void);
void OP_rtt(void);

void OP_wait(void);
void OP_reset(void);

void OP_cc(u16 flgs);

#endif