#ifndef _cpu_intrl_h
#define _cpu_intrl_h

#include "cpu_idefs.h"

void INTsighndlr1(int signo);
void INTsighndlr2(int signo);
void INTsighndlr3(int signo);

u16 INTgetaddr(u8 bm, u8 b5, u8 b4, u8 b3, u8 b2, u8 b1, u8 b0);
u8 INTcalcoff(u8 b7, u8 b6, u8 b5, u8 b4, u8 b3, u8 b2, u8 b1, u8 b0);
u8 INTgetregnum(u8 b2, u8 b1, u8 b0);

void INTu16_to_word(u16 val, WORD &res);

void INTbyteZ(u8 val);
void INTbyteN(u8 val);
void INTbyteC(u8 val, u8 val2);
void INTbyteV(u8 val, u8 val2);

void INTwordZ(u16 val);
void INTwordN(u16 val);
void INTwordC(u16 val, u16 val2);
void INTwordV(u16 val, u16 val2);

void INTpush(u16 val);
void INTpop(u16 &val);
void INTpeek(u16 &val);

s8 INTu8tos8(u8 val);
u8 INTs8tou8(s8 val);
s16 INTu16tos16(u16 val);
u16 INTs16tou16(s16 val);

#endif

