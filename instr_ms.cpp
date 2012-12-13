/* instr_ms.cpp - Miscellaneous Instructions
 * 
 * Prefix: OP
 *
 * This file contains all the functions than
 * emulatate the miscellaneous instructions 
 * of the PDP-11. They all are of varied
 * formats.
 *
 * Written By: Roland Rabien
 */
#include "cpu.h"
#include "cpu_intrl.h"
#include "cpu_idefs.h"

extern u16 regs[NUMREGS];
extern int thistick;

void OP_jmp(u16 addr)
{
  u16 val;

  CPUreadword(addr, val);

  regs[PC] = val;
}

void OP_jsr(u16 rr, u16 dd)
{
  u16 sbr = 0;
  u16 reg = 0;

  CPUreadword(dd, sbr);

  INTpush(reg);

  regs[PC] += 2;  
  CPUwriteword(rr, regs[PC]);

  regs[PC] = sbr;
}

void OP_rts(u16 addr)
{
  u16 reg;

  CPUreadword(addr, reg);

  regs[PC] = reg;

  INTpop(reg);

  CPUwriteword(addr, reg);
}

void OP_mark(u16 nn)
{
  u16 val;

  regs[SP] = regs[PC] + (2 * nn);

  regs[PC] = regs[R5];

  INTpop(val);

  regs[R5] = val;
}

void OP_sob(u16 rr, u16 offset)
{
  u16 reg;

  CPUreadword(rr, reg);
  reg--;
  CPUwriteword(rr, reg);

  if (reg == 0)
  {
    regs[PC] += 2;
  }
  else
  {
    regs[PC] = regs[PC] - (2 * offset);
  }
}

void OP_emt(void)
{
  CPUinterrupt(0x18);
}

void OP_trap(void)
{
  CPUinterrupt(0x1C);
}

void OP_bpt(void)
{
  CPUinterrupt(0x0C);
}

void OP_iot(void)
{
  CPUinterrupt(0x10);
}

void OP_rti(void)
{
  u16 val;

  INTpop(val);
  regs[PC] = val;

  INTpop(val);
  regs[PSW] = val;
}

void OP_rtt(void)
{
  u16 val;

  INTpop(val);
  regs[PC] = val;

  INTpop(val);
  regs[PSW] = val;
}

void OP_wait(void)
{
  // This function does nothing: we have no bus emulator.
	thistick -= IPERTICK;
}

void OP_reset(void)
{
	thistick -= IPERTICK;
}

void OP_cc(u16 flgs)
{
  bool val;

  if (flgs & 0x0010)
  {
    val = true;
  }
  else
  {
    val = false;
  }

  if (flgs & 0x0008) CPUsetflag(FN, val);
  if (flgs & 0x0004) CPUsetflag(FZ, val);
  if (flgs & 0x0002) CPUsetflag(FV, val);
  if (flgs & 0x0001) CPUsetflag(FC, val);
}
