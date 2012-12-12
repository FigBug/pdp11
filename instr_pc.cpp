/* instr_pc.cpp - Program Counter Instructions
 * 
 * Prefix: OP
 *
 * This file contains all the functions than
 * emulatate the program counter instructions 
 * of the PDP-11. They all are of the folling
 * format:
 *
 * void OP_<instruction name>(<signed offset>)
 *
 * Written By: Roland Rabien
 */

#include "cpu.h"
#include "cpu_intrl.h"
#include "cpu_idefs.h"

extern u16 regs[NUMREGS];

void OP_br(u8 off)
{
  s8 offset;

  offset = INTu8tos8(off);

  regs[PC] = regs[PC] + (offset * 2) + 2;
}

void OP_bne(u8 off)
{
  s8 offset;
  bool cc;

  offset = INTu8tos8(off);

  CPUtstflag(FZ, cc);

  if (cc)
  {
    regs[PC] += 2;
  }
  else
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;
  }
}

void OP_beq(u8 off)
{
  s8 offset;
  bool cc;

  CPUtstflag(FZ, cc);

  offset = INTu8tos8(off);

  if (cc)
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;
  }
  else
  {
    regs[PC] += 2;
  }
}

void OP_bpl(u8 off)
{
  s8 offset;
  bool cc;

  CPUtstflag(FN, cc);

  offset = INTu8tos8(off);

  if (cc)
  {
    regs[PC] += 2;
  }
  else
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;    
  }
}

void OP_bmi(u8 off)
{
  s8 offset;
  bool cc;

  CPUtstflag(FN, cc);

  offset = INTu8tos8(off);

  if (cc)
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;    
  }
  else
  {
    regs[PC] += 2;    
  }
}

void OP_bvc(u8 off)
{
  s8 offset;
  bool cc;

  CPUtstflag(FV, cc);

  offset = INTu8tos8(off);

  if (cc)
  {    
    regs[PC] += 2;    
  }
  else
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;    
  }
}

void OP_bvs(u8 off)
{
  s8 offset;
  bool cc;

  offset = INTu8tos8(off);

  CPUtstflag(FV, cc);

  if (cc)
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;    
  }
  else
  {
    regs[PC] += 2;    
  }
}

void OP_bcc(u8 off)
{
  s8 offset;
  bool cc;

  offset = INTu8tos8(off);

  CPUtstflag(FC, cc);

  if (cc)
  {
    regs[PC] += 2;        
  }
  else
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;    
  }
}

void OP_bcs(u8 off)
{
  s8 offset;
  bool cc;

  offset = INTu8tos8(off);

  CPUtstflag(FC, cc);

  if (cc)
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;    
  }
  else
  {
    regs[PC] += 2;    
  }
}

void OP_bge(u8 off)
{
  s8 offset;
  bool nn;
  bool vv;

  offset = INTu8tos8(off);

  CPUtstflag(FV, vv);
  CPUtstflag(FN, nn);

  if (nn || vv && (!(nn && vv)))
  {    
    regs[PC] += 2;    
  }
  else
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;    
  }
}

void OP_blt(u8 off)
{
  s8 offset;
  bool nn;
  bool vv;

  offset = INTu8tos8(off);

  CPUtstflag(FV, vv);
  CPUtstflag(FN, nn);

  if (nn || vv && (!(nn && vv)))
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;    
  }
  else
  {
    regs[PC] += 2;    
  }
}

void OP_bgt(u8 off)
{
  s8 offset;
  bool nn;
  bool vv;
  bool zz;

  offset = INTu8tos8(off);

  CPUtstflag(FV, vv);
  CPUtstflag(FN, nn);
  CPUtstflag(FZ, zz);

  if (zz || (nn || vv && (!(nn && vv))))
  {
    regs[PC] += 2;    
  }
  else
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;    
  }
}

void OP_ble(u8 off)
{
  s8 offset;
  bool nn;
  bool vv;
  bool zz;

  offset = INTu8tos8(off);

  CPUtstflag(FV, vv);
  CPUtstflag(FN, nn);
  CPUtstflag(FZ, zz);

  if (zz || (nn || vv && (!(nn && vv))))
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;    
  }
  else
  {
    regs[PC] += 2;    
  }
}

void OP_bhi(u8 off)
{
  s8 offset;
  bool cc;
  bool zz;

  offset = INTu8tos8(off);

  CPUtstflag(FC, cc);
  CPUtstflag(FZ, zz);

  if ((cc == 0) && (zz == 0))
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;    
  }
  else
  {
    regs[PC] += 2;    
  }
}

void OP_blos(u8 off)
{
  s8 offset;
  bool cc;
  bool zz;

  offset = INTu8tos8(off);

  CPUtstflag(FC, cc);
  CPUtstflag(FZ, zz);

  if (cc || zz)
  {
    regs[PC] = regs[PC] + (offset * 2) + 2;    
  }
  else
  {
    regs[PC] += 2;    
  }
}
