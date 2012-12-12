/* cpu_intrl.cpp - internal utilities module
 *
 * Prefix: INT
 *
 * This module contains simple utilities used
 * in all other parts of the PDP-11 emnulator
 *
 * Written By: Roland Rabien
 */

#include <stdio.h>
#include <signal.h>

#include "cpu.h"
#include "cpu_idefs.h"

#include "cpu_intrl.h"

extern u16 regs[NUMREGS];

extern int clocktick;
extern int thistick;
extern int run;
extern u16 clkreg;

/* Convert a u16 to a word data structure */
void INTu16_to_word(u16 val, WORD &res)
{
  int cnt;
  int bit;

  bit = 1;

  for (cnt = 0; cnt < WORD_SZ; cnt++)
  {
    if (val & bit)
    {
      res[cnt] = 1;
    }
    else
    {
      res[cnt] = 0;
    }
    bit = bit << 1;
  }
}

/* Given three bits, calculate their value */
u8 INTgetregnum(u8 b2, u8 b1, u8 b0)
{
  return ((b2 * 4) + (b1 * 2) + (b0 * 1));
}

/* Given eight bits, calculate their value */
u8 INTcalcoff(u8 b7, u8 b6, u8 b5, u8 b4, u8 b3, u8 b2, u8 b1, u8 b0)
{
  return (b7 * 128) + (b6 * 64) + (b5 * 32) + (b4 * 16) + (b3 * 8) + (b2 * 4) + (b1 * 2) + (b0 * 1);
}

/* Given 6 bits plus a Byte/Word mode, calculate the address 
 * 
 * - used to find what memory addresses an op code i working on
 */
u16 INTgetaddr(u8 bm, u8 b5, u8 b4, u8 b3, u8 b2, u8 b1, u8 b0)
{
  u8 regnum;
  u16 addr;
  u16 defaddr;

  regnum = INTgetregnum(b2, b1, b0);

  /* Reg = 7 (PC) */
  if (b2 == 1 && b1 == 1 && b0 == 1)
  {
		thistick--;
    /* Mode 2: Immediate */
    if (b5 == 0 && b4 == 1 && b3 == 0)
    {
      regs[PC] += 2;

      if (bm)
      {
        return regs[PC] + 1;
      }
      else
      {
        return regs[PC];
      }
    }
    /* Mode 3: Absolute */
    if (b5 == 0 && b4 == 1 && b3 == 1)
    {
      regs[PC] += 2;

      CPUreadword(regs[PC], addr);

      return addr;
    }
    /* Mode 6: Relative */
    if (b5 == 1 && b4 == 1 && b3 == 0)
    {
      regs[PC] += 2;

      CPUreadword(regs[PC], addr);

      addr = addr + regs[PC] + 2;

      return defaddr;
    }
    /* Mode 7: Relative Deferred */
    if (b5 == 1 && b4 == 1 && b3 == 1)
    {
      regs[PC] += 2;

      CPUreadword(regs[PC], addr);

      addr = addr + regs[PC] + 2;

      CPUreadword(addr, defaddr);

      return defaddr;
    }
  }

  /* mode 0: register */
  if (b5 == 0 && b4 == 0 && b3 == 0)
  {
    if (b2 == 0 && b1 == 0 && b0 == 0) return M_R0;
    if (b2 == 0 && b1 == 0 && b0 == 1) return M_R1;
    if (b2 == 0 && b1 == 1 && b0 == 0) return M_R2;
    if (b2 == 0 && b1 == 1 && b0 == 1) return M_R3;
    if (b2 == 1 && b1 == 0 && b0 == 0) return M_R4;
    if (b2 == 1 && b1 == 0 && b0 == 1) return M_R5;
    if (b2 == 1 && b1 == 1 && b0 == 0) return M_R6;
    if (b2 == 1 && b1 == 1 && b0 == 1) return M_R7;    
  }
  thistick--;
  /* mode 1: register deferred */
  if (b5 == 0 && b4 == 0 && b3 == 1)
  {
    return (regs[regnum]);
  }
  /* mode 2: autoincrement */
  if (b5 == 0 && b4 == 1 && b3 == 0)
  {
    addr = regs[regnum];

    if (bm)
    {
      regs[regnum] ++;
    }
    else
    {
      regs[regnum] += 2;
    }
    return addr;
  }
  /* mode 3: autoincrement deferred */
  if (b5 == 0 && b4 == 1 && b3 == 1)
  {
    addr = regs[regnum];

    CPUreadword(addr, defaddr);

    regs[regnum] += 2;

    return defaddr;
  }
  /* mode 4: autodecrement */
  if (b5 == 1 && b4 == 0 && b3 == 0)
  {
    addr = regs[regnum];

    if (bm)
    {
      regs[regnum] --;
    }
    else
    {
      regs[regnum] -= 2;
    }
    return addr;
  }
  /* mode 5: autodecrement defrrered */
  if (b5 == 1 && b4 == 0 && b3 == 1)
  {
    addr = regs[regnum];
    CPUreadword(addr, defaddr);

    regs[regnum] -= 2;

    return defaddr;
  }
  /* mode 6: index */
  if (b5 == 1 && b4 == 1 && b3 == 0)
  {
    regs[PC] += 2;
    CPUreadword(regs[PC], addr);
    addr += regs[regnum];

    return addr;
  }
  /* mode 7: index defrred */
  if (b5 == 1 && b4 == 1 && b3 == 1)
  {
    regs[PC] += 2;
    CPUreadword(regs[PC], addr);
    addr += regs[regnum];
    CPUreadword(addr, defaddr);

    return defaddr;
  }
  return 0;
}

/* Sets Z flag, depending on byte value */
void INTbyteZ(u8 val)
{
  if (val == 0)
  {
    CPUsetflag(FZ, set);
  }
  else
  {
    CPUsetflag(FZ, clear);
  }
}

/* Sets N flag, depending on byte value */
void INTbyteN(u8 val)
{
  if (val & 0x80)
  {
    CPUsetflag(FN, set);
  }
  else
  {
    CPUsetflag(FN, clear);
  }
}

/* Sets C flag, if bytes val == val2 */
void INTbyteC(u8 val, u8 val2)
{
  if (val == val2)
  {
    CPUsetflag(FC, set);
  }
  else
  {
    CPUsetflag(FC, clear);
  }
}

/* Sets V flag, if bytes val == val2 */
void INTbyteV(u8 val, u8 val2)
{
  if (val == val2)
  {
    CPUsetflag(FV, set);
  }
  else
  {
    CPUsetflag(FV, clear);
  }
}

/* Sets Z flag, depending on word value */
void INTwordZ(u16 val)
{
  if (val == 0)
  {
    CPUsetflag(FZ, set);
  }
  else
  {
    CPUsetflag(FZ, clear);
  }
}

/* Sets N flag, depending on word value */
void INTwordN(u16 val)
{
  if (val & 0x8000)
  {
    CPUsetflag(FN, set);
  }
  else
  {
    CPUsetflag(FN, clear);
  }
}

/* Set C flag, if words val == val2 */
void INTwordC(u16 val, u16 val2)
{
  if (val == val2)
  {
    CPUsetflag(FC, set);
  }
  else
  {
    CPUsetflag(FC, clear);
  }
}

/* Set V flag, in words val == val2 */
void INTwordV(u16 val, u16 val2)
{
  if (val == val2)
  {
    CPUsetflag(FV, set);
  }
  else
  {
    CPUsetflag(FV, clear);
  }
}

/* Push a value onto the stack */
void INTpush(u16 val)
{
  regs[SP] -= 2;

  CPUwriteword(regs[SP], val);
}

/* Pop a value off of the stack */
void INTpop(u16 &val)
{
  CPUreadword(regs[SP], val);

  regs[SP] += 2;
}

/* Look at the top item on the stack */
void INTpeek(u16 &val)
{
  CPUreadword(regs[SP], val);
}

/* Convert u8 to s8 */
s8 INTu8tos8(u8 val)
{
  if (val > 127)
  {
    return (val - 256);
  }
  else
  {
    return val;
  }
}

/* Convert s8 to u8 */
u8 INTs8tou8(s8 val)
{
  if (val < 0)
  {
    return (val + 256);
  }
  else
  {
    return val;
  }
}

/* Convert u16 to s16 */
s16 INTu16tos16(u16 val)
{
  if (val > 32767)
  {
    return (val - 65536);
  }
  else
  {
    return val;
  }
}

/* Convert s16 to u16 */
u16 INTs16tou16(s16 val)
{
  if (val < 0)
  {
    return (val + 65536);
  }
  else
  {
    return val;
  }
}

void INTsighndlr1(int signo)
{
	clocktick = 1;
  clkreg = clkreg | IO_RDY;

  if (clkreg & IO_INT)
  {
    CPUinterrupt(0x40);
  }
}

void INTsighndlr2(int signo)
{
	clocktick = -1;
}

void INTsighndlr3(int signo)
{
  run = 0;
}
