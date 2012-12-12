/* instr_do.cpp - Double Operand Instructions
 * 
 * Prefix: OP
 *
 * This file contains all the functions than
 * emulatate the double operand instructions 
 * of the PDP-11. They all are of the folling
 * format:
 *
 * void OP_<instruction name>(<source addr>, <destination address>)
 *
 * Written By: Roland Rabien
 */

#include "cpu.h"
#include "cpu_intrl.h"
#include "cpu_idefs.h"

void OP_bit(u16 src, u16 dst)
{
  u16 ss;
  u16 dd;

  CPUreadword(src, ss);
  CPUreadword(dst, dd);

  dd = ss & dd;

  INTwordN(dd);
  INTwordZ(dd);
  CPUsetflag(FV, clear);
}

void OP_bitb(u16 src, u16 dst)
{
  u8 ss;
  u8 dd;

  CPUreadbyte(src, ss);
  CPUreadbyte(dst, dd);

  dd = ss & dd;

  INTbyteN(dd);
  INTbyteZ(dd);
  CPUsetflag(FV, clear);
}

void OP_bic(u16 src, u16 dst)
{
  u16 ss;
  u16 dd;

  CPUreadword(src, ss);
  CPUreadword(dst, dd);

  dd = ~ss & dd;

  CPUwriteword(dst, dd);

  INTwordN(dd);
  INTwordZ(dd);
  CPUsetflag(FV, clear);
}

void OP_bicb(u16 src, u16 dst)
{
  u8 ss;
  u8 dd;

  CPUreadbyte(src, ss);
  CPUreadbyte(dst, dd);

  dd = ~ss & dd;

  CPUwriteword(dst, dd);

  INTbyteN(dd);
  INTbyteZ(dd);
  CPUsetflag(FV, clear);
}

void OP_bis(u16 src, u16 dst)
{
  u16 ss;
  u16 dd;

  CPUreadword(src, ss);
  CPUreadword(dst, dd);

  dd = ss | dd;

  CPUwriteword(dst, dd);

  INTwordN(dd);
  INTwordZ(dd);
  CPUsetflag(FV, clear);
}

void OP_bisb(u16 src, u16 dst)
{
  u8 ss;
  u8 dd;

  CPUreadbyte(src, ss);
  CPUreadbyte(dst, dd);

  dd = ss | dd;

  CPUwriteword(dst, dd);

  INTbyteN(dd);
  INTbyteZ(dd);
  CPUsetflag(FV, clear);
}

void OP_add(u16 src, u16 dst)
{
  u16 sval;
  u16 dval;
  s16 ss;
  s16 dd;
  s16 rr;

  CPUreadword(src, sval);
  CPUreadword(dst, dval);

  ss = INTu16tos16(sval);
  dd = INTu16tos16(dval);

  rr = ss + dd;

  dval = INTs16tou16(rr);
  CPUwriteword(dst, dval);

  if ((dd > 0 && ss > 0 && rr < 0) ||
      (dd < 0 && ss < 0 && rr > 0))
  {
    CPUsetflag(FV, set);
  }
  else
  {
    CPUsetflag(FV, clear);
  }

  INTwordN(dval);
  INTwordZ(dval);
}

void OP_sub(u16 src, u16 dst)
{
  u16 sval;
  u16 dval;
  s16 ss;
  s16 dd;
  s16 rr;

  CPUreadword(src, sval);
  CPUreadword(dst, dval);

  ss = INTu16tos16(sval);
  dd = INTu16tos16(dval);

  rr = ss - dd;

  dval = INTs16tou16(rr);
  CPUwriteword(dst, dval);

  if ((dd > 0 && ss > 0 && rr < 0) ||
      (dd < 0 && ss < 0 && rr > 0))
  {
    CPUsetflag(FV, set);
  }
  else
  {
    CPUsetflag(FV, clear);
  }

  INTwordN(dval);
  INTwordZ(dval);
}

void OP_mov(u16 src, u16 dst)
{
  u16 temp;

  CPUreadword(src, temp);
  CPUwriteword(dst, temp);

  INTwordZ(temp);
  INTwordN(temp);
}

void OP_movb(u16 src, u16 dst)
{
  u8 temp;

  CPUreadbyte(src, temp);
  CPUwritebyte(dst, temp);

  INTbyteZ(temp);
  INTbyteN(temp);
}

void OP_cmp(u16 src, u16 dst)
{
  u16 ss;
  u16 dd;

  CPUreadword(src, ss);
  CPUreadword(dst, dd);

  ss = ss - dd;

  INTwordN(ss);
  INTwordZ(ss);
}

void OP_cmpb(u16 src, u16 dst)
{
  u8 ss;
  u8 dd;

  CPUreadbyte(src, ss);
  CPUreadbyte(dst, dd);

  ss = ss - dd;

  INTbyteN(ss);
  INTbyteZ(ss);
}

void OP_xor(u16 src, u16 dst)
{
  u16 ss;
  u16 dd;

  CPUreadword(src, ss);
  CPUreadword(dst, dd);

  dd = ss ^ dd;

  CPUwriteword(dst, dd);

  INTwordN(dd);
  INTwordZ(dd);
  CPUsetflag(FV, clear);  
}

void OP_mul(u16 src, u16 dst)
{
  u16 sval;
  u16 dval;

  u16 hival;
  u16 loval;

  s16 ss;
  s16 dd;
  s32 rr;

  s16 hi;
  s16 lo;

  CPUreadword(src, sval);
  CPUreadword(dst, dval);

  ss = INTu16tos16(sval);
  dd = INTu16tos16(dval);

  rr = (u32)ss * (u32)dd;

  lo = rr & 0x0000FFFF;
  hi = (u16)(rr >> 16);

  rr < 0 ? CPUsetflag(FN, set) : CPUsetflag(FN, clear);
  rr == 0 ? CPUsetflag(FZ, set) : CPUsetflag(FZ, clear);
  rr < -32768 || rr > 2 ? CPUsetflag(FC, set) : CPUsetflag(FC, clear);

  CPUsetflag(FV, clear);

  hival = INTs16tou16(hi);
  loval = INTs16tou16(lo);

  if (src % 2 == 0)
  {
    CPUwriteword(src, lo);
    CPUwriteword(src+1, hi);
  }
  else
  {
    CPUwriteword(src, lo);
  }
}

void OP_div(u16 src, u16 dst)
{
  u16 r1;
  u16 r2;
  u16 ss;
  u32 rr;

  s32 *top;
  s16 *bot;

  CPUreadword(src, r1);
  CPUreadword(src+1, r2);
  CPUreadword(dst, ss);

  rr = (r1 << 16) + r2;

  top = (s32 *)&rr;
  bot = (s16 *)&ss;

  *bot = *top / *bot;

  INTwordN(*bot);
  INTwordZ(*bot);
  
  ss == 0 ? CPUsetflag(FV, set) : CPUsetflag(FV, clear);
  bot == 0 ? CPUsetflag(FC, set) : CPUsetflag(FC, clear);

  CPUwriteword(src, ss);
}
