/* instr_so.cpp - Single Operand Instructions
 * 
 * Prefix: OP
 *
 * This file contains all the functions than
 * emulatate the single operand instructions 
 * of the PDP-11. They all are of the folling
 * format:
 *
 * void OP_<instruction name>(<source addr>)
 *
 * Written By: Roland Rabien
 */

#include "cpu.h"

#include "instr_so.h"
#include "cpu_intrl.h"

extern u16 regs[NUMREGS];

void OP_clr(u16 addr)
{
  CPUwriteword(addr, 0x0000);

  CPUsetflag(FN, clear);
  CPUsetflag(FZ, set);
  CPUsetflag(FV, clear);
  CPUsetflag(FC, clear);
}

void OP_inc(u16 addr)
{
  u16 val;

  CPUreadword(addr, val);

  INTwordV(val, 0x7FFF);

  val++;

  CPUwriteword(addr, val);

  INTwordN(val);
  INTwordZ(val);
}

void OP_incb(u16 addr)
{
  u8 val;

  CPUreadbyte(addr, val);

  INTbyteV(val, 0x7F);

  val++;

  CPUwritebyte(addr, val);

  INTbyteN(val);
  INTbyteZ(val);
}

void OP_clrb(u16 addr)
{
  CPUwritebyte(addr, 0x00);

  CPUsetflag(FN, clear);
  CPUsetflag(FZ, set);
  CPUsetflag(FV, clear);
  CPUsetflag(FC, clear);
}

void OP_com(u16 addr)
{
  u16 val;

  CPUreadword(addr, val);

  val = ~val;

  CPUwriteword(addr, val);

  INTwordN(val);
  INTwordZ(val);
  CPUsetflag(FV, clear);
  CPUsetflag(FC, set);
}

void OP_comb(u16 addr)
{
  u8 val;

  CPUreadbyte(addr, val);

  val = ~val;

  CPUwritebyte(addr, val);

  INTbyteN(val);
  INTbyteZ(val);
  CPUsetflag(FV, clear);
  CPUsetflag(FC, set);
}

void OP_dec(u16 addr)
{
  u16 val;

  CPUreadword(addr, val);

  INTwordV(val, 0x8000);

  val--;

  CPUwriteword(addr, val);

  INTwordN(val);
  INTwordZ(val);
}

void OP_decb(u16 addr)
{
  u8 val;

  CPUreadbyte(addr, val);

  INTwordV(val, 0x80);

  val--;

  CPUwritebyte(addr, val);

  INTbyteN(val);
  INTbyteZ(val);
}

void OP_neg(u16 addr)
{
  u16 val;
  s16 *valp;

  valp = (s16 *)&val;

  CPUreadword(addr, val);

  *valp = -(*valp);

  CPUwriteword(addr, val);

  INTwordN(*valp);
  INTwordZ(*valp);
  INTwordV(*valp, 0x8000);
  INTwordC(*valp, 0);
}

void OP_negb(u16 addr)
{
  u8 val;
  s8 *valp;

  valp = (s8 *)&val;

  CPUreadbyte(addr, val);

  *valp = -(*valp);

  CPUwritebyte(addr, val);

  INTbyteN(*valp);
  INTbyteZ(*valp);
  INTbyteV(*valp, 0x80);
  INTbyteC(*valp, 0);
}

void OP_tst(u16 addr)
{
  u16 val;

  CPUreadword(addr, val);

  INTwordN(val);
  INTwordZ(val);
  CPUsetflag(FV, clear);
  CPUsetflag(FC, clear);
}

void OP_tstb(u16 addr)
{
  u8 val;

  CPUreadbyte(addr, val);

  INTbyteN(val);
  INTbyteZ(val);
  CPUsetflag(FV, clear);
  CPUsetflag(FC, clear);
}

void OP_asr(u16 addr)
{
  u16 val;
  bool n = false;
  bool c = false;

  CPUreadword(addr, val);

  0x0001 & val ? CPUsetflag(FC, set) : CPUsetflag(FC, clear);

  val = val >> 1;

  if (0x4000 & val)
  {
    val = val | 0x8000;
  }

  CPUwriteword(addr, val);

  INTwordN(val);
  INTwordZ(val);

  if ((c || n) && (!(c && n)))
  {
    CPUsetflag(FV, set);
  }
  else
  {
    CPUsetflag(FV, clear);
  }
}

void OP_asrb(u16 addr)
{
  u8 val;
  bool n = false;
  bool c = false;

  CPUreadbyte(addr, val);

  0x0001 & val ? CPUsetflag(FC, set) : CPUsetflag(FC, clear);

  val = val >> 1;

  if (0x40 & val)
  {
    val = val | 0x80;
  }

  CPUwritebyte(addr, val);

  INTbyteN(val);
  INTbyteZ(val);

  if ((c || n) && (!(c && n)))
  {
    CPUsetflag(FV, set);
  }
  else
  {
    CPUsetflag(FV, clear);
  }
}

void OP_asl(u16 addr)
{
  u16 val;
  bool n;
  bool c;

  CPUreadword(addr, val);

  val = val << 1;

  CPUwriteword(addr, val);

  INTwordN(val);
  INTwordZ(val);
  0x8000 & val ? CPUsetflag(FC, set) : CPUsetflag(FC, clear);

  CPUtstflag(FN, n);
  CPUtstflag(FC, c);

  if ((c || n) && (!(c && n)))
  {
    CPUsetflag(FV, set);
  }
  else
  {
    CPUsetflag(FV, clear);
  }
}

void OP_aslb(u16 addr)
{
  u8 val;
  bool n;
  bool c;

  CPUreadbyte(addr, val);

  val = val << 1;

  CPUwritebyte(addr, val);

  INTbyteN(val);
  INTbyteZ(val);
  0x80 & val ? CPUsetflag(FC, set) : CPUsetflag(FC, clear);

  CPUtstflag(FN, n);
  CPUtstflag(FC, c);

  if ((c || n) && (!(c && n)))
  {
    CPUsetflag(FV, set);
  }
  else
  {
    CPUsetflag(FV, clear);
  }
}

void OP_ror(u16 addr)
{
  u16 val;
  u16 res;
  bool n = false;
  bool c = false;
  bool cc = false;

  CPUreadword(addr, val);

  res = val >> 1;

  CPUtstflag(FC, cc);

  if (cc)
  {
    res = res | 0x8000;
  }

  CPUwriteword(addr, res);

  0x0001 & val ? CPUsetflag(FC, set) : CPUsetflag(FC, clear);

  INTwordN(val);
  INTwordZ(val);

  if ((c || n) && (!(c && n)))
  {
    CPUsetflag(FV, set);
  }
  else
  {
    CPUsetflag(FV, clear);
  }
}

void OP_rorb(u16 addr)
{
  u8 val;
  u8 res;
  bool n = false;
  bool c = false;
  bool cc = false;

  CPUreadbyte(addr, val);

  res = val >> 1;

  CPUtstflag(FC, cc);

  if (cc)
  {
    res = res | 0x80;
  }

  CPUwritebyte(addr, res);

  0x01 & val ? CPUsetflag(FC, set) : CPUsetflag(FC, clear);

  INTbyteN(val);
  INTbyteZ(val);

  if ((c || n) && (!(c && n)))
  {
    CPUsetflag(FV, set);
  }
  else
  {
    CPUsetflag(FV, clear);
  }
}

void OP_rol(u16 addr)
{
  u16 val;
  u16 res;
  bool n = false;
  bool c = false;
  bool cc = false;

  CPUreadword(addr, val);

  res = val << 1;

  CPUtstflag(FC, cc);

  if (cc)
  {
    res = res | 0x0001;
  }

  CPUwriteword(addr, res);

  0x8000 & val ? CPUsetflag(FC, set) : CPUsetflag(FC, clear);

  INTwordN(val);
  INTwordZ(val);

  if ((c || n) && (!(c && n)))
  {
    CPUsetflag(FV, set);
  }
  else
  {
    CPUsetflag(FV, clear);
  }
}

void OP_rolb(u16 addr)
{
  u8 val;
  u8 res;
  bool n = false;
  bool c = false;
  bool cc = false;

  CPUreadbyte(addr, val);

  res = val << 1;

  CPUtstflag(FC, cc);

  if (cc)
  {
    res = res | 0x01;
  }

  CPUwritebyte(addr, res);

  0x80 & val ? CPUsetflag(FC, set) : CPUsetflag(FC, clear);

  INTbyteN(val);
  INTbyteZ(val);

  if ((c || n) && (!(c && n)))
  {
    CPUsetflag(FV, set);
  }
  else
  {
    CPUsetflag(FV, clear);
  }
}

void OP_swab(u16 addr)
{
  u8 hi;
  u8 lo;

  CPUreadbyte(addr, hi);
  CPUreadbyte(addr+1, lo);

  CPUwritebyte(addr, lo);
  CPUwritebyte(addr+1, hi);

  INTbyteN(lo);
  INTbyteZ(lo);
  CPUsetflag(FV, clear);
  CPUsetflag(FC, clear);
}

void OP_adc(u16 addr)
{
  u16 val;
  bool cc;

  CPUreadword(addr, val);
  CPUtstflag(FC, cc);

  if (cc)
  {
    INTwordV(val, 0x7FFF);
    INTwordC(val, 0xFFFF);
    val++;
    CPUwriteword(addr, val);
  }
  else
  {
    CPUsetflag(FV, clear);
    CPUsetflag(FC, clear);
  }
  INTwordN(val);
  INTwordZ(val);
}

void OP_adcb(u16 addr)
{
  u8 val;
  bool cc;

  CPUreadbyte(addr, val);
  CPUtstflag(FC, cc);

  if (cc)
  {
    INTbyteV(val, 0x7F);
    INTbyteC(val, 0xFF);
    val++;
    CPUwritebyte(addr, val);
  }
  else
  {
    CPUsetflag(FV, clear);
    CPUsetflag(FC, clear);
  }
  INTbyteN(val);
  INTbyteZ(val);
}

void OP_sbc(u16 addr)
{
  u16 val;
  bool cc;

  CPUreadword(addr, val);
  CPUtstflag(FC, cc);
  INTwordV(val, 0x8000);

  if (cc)
  {    
    INTwordC(val, 0x0000);
    val--;
    CPUwriteword(addr, val);
  }
  else
  {    
    CPUsetflag(FC, clear);
  }
  INTwordN(val);
  INTwordZ(val);
}

void OP_sbcb(u16 addr)
{
  u8 val;
  bool cc;

  CPUreadbyte(addr, val);
  CPUtstflag(FC, cc);
  INTbyteV(val, 0x80);

  if (cc)
  {    
    INTbyteC(val, 0x00);
    val--;
    CPUwritebyte(addr, val);
  }
  else
  {    
    CPUsetflag(FC, clear);
  }
  INTbyteN(val);
  INTbyteZ(val);
}

void OP_sxt(u16 addr)
{
  bool cc;

  CPUtstflag(FN, cc);

  if (cc)
  {
    CPUwriteword(addr, 0xFFFF);
    CPUsetflag(FN, clear);
  }
  else
  {
    CPUwriteword(addr, 0x0000);
    CPUsetflag(FN, set);
  }
  CPUsetflag(FV, clear);
}

void OP_mfps(u16 addr)
{
  u16 val;
  u8 dd;

  val = regs[PSW] & 0x00FF;
  dd = (u8)val;

  if (val & 0x0080)
  {
    val = val | 0xFF00;
  }

  if (addr >= M_R0 && addr <= M_R7)
  {
    CPUwriteword(addr, val);
  }
  else
  {    
    CPUwritebyte(addr, dd);
  }

  INTbyteN(dd);
  INTbyteZ(dd);
  CPUsetflag(FV, clear);
}

void OP_mtps(u16 addr)
{
  u8 val;

  CPUreadbyte(addr, val);

  regs[PSW] = regs[PSW] & 0xFF00;

  regs[PSW] += val;

  CPUsetflag(FT, clear);
}
