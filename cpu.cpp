/* cpu.cpp - main module
 *
 * Prefix: CPU
 *
 * This is the main module of the PDP-11 Emulator
 * it contains all the externaly visable functions.
 *
 * Written By: Roland Rabien
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "cpu.h"
#include "cpu_idefs.h"
#include "cpu_intrl.h"

#include "instr_so.h"
#include "instr_do.h"
#include "instr_pc.h"
#include "instr_ms.h"

#include "io.h"

/* Global Variables */

u8 ram[MEM_SZ*(WORD_SZ/BPSU)*KB];

u16 regs[NUMREGS];

BREAK_PNT brkpnts[MAXBRK];

u16 clkreg;
int run;
int out;
int in;

extern u16 Ostatusreg;
extern u16 Istatusreg;

/* Stuff For The Clock */

int clocktick;

int thistick;

/* some interrupt stuff */

struct waitint {
  int vector;
  int priority;
};

int intswaiting;

struct waitint intlist[20];

/* Global Functions */

bool CPUinit(void)
{
  int cnt;
  int pid;

  char outPipeName[64];
  char inPipeName[64];

	struct sigaction sigact;
  clocktick = -1;

  /* init: break points */
  for (cnt = 0; cnt < MAXBRK; cnt++)
  {
    brkpnts[cnt].enable = false;
    brkpnts[cnt].addr = 0;
  }
  /* init: memory */
  for (cnt = 0; cnt < MEM_SZ*(WORD_SZ/BPSU)*KB; cnt++)
  {
    ram[cnt] = 0;
  }
  /* init break points */
  for (cnt = 0; cnt < NUMREGS; cnt++)
  {
    regs[cnt] = 0;
  }
  regs[PC] = 512;
  regs[SP] = 510;

  /* Report fatal error if variable sizes are incorrect */
  if ((sizeof(u8) != 1)  || (sizeof(s8) != 1) ||
      (sizeof(u16) != 2) || (sizeof(s16) != 2))
  {
    printf("Fatal Errors in compile: Barfing Hardcore!\n");
    return true;
  }

	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_RESTART;

	sigact.sa_handler = INTsighndlr1;
	sigaction(SIGUSR1, &sigact, NULL);

	sigact.sa_handler = INTsighndlr2;
	sigaction(SIGUSR2, &sigact, NULL);

	sigact.sa_handler = INTsighndlr3;
	sigaction(SIGINT, &sigact, NULL);

  printf("Syncing output\n");
  pid = getpid();
  sprintf(outPipeName, "pdp%d_out", pid);
  mkfifo(outPipeName, S_IRUSR | S_IWUSR);
  out = open(outPipeName, O_WRONLY);

  printf("Syncing input\n");
  sprintf(inPipeName, "pdp%d_in", pid);
  mkfifo(inPipeName, S_IRUSR | S_IWUSR);
  in = open(inPipeName, O_RDONLY);

  printf("\nPDP-11/04 Emulator: kicking it up a notch!\n");

  return false;
}

bool CPUhalt(void)
{
  int pid;
  char pipename[64];

  printf("\nPDP-11/04 Emulator: Word to your mother!\n");

  close(in);
  close(out);

  pid = getpid();
  sprintf(pipename, "pdp%d_in", pid);
  remove(pipename);
  sprintf(pipename, "pdp%d_out", pid);
  remove(pipename);
  
  return false;
}

bool CPUreadword(u16 addr, u16 &word)
{
  u8 hi;
  u8 lo;

  if (addr % 2 == 0)
  {
    if (addr < 57344)
    {
      lo = ram[addr+1];
      hi = ram[addr];

      word = hi << BYTE_SZ;
      word = word + lo;

      return false;
    }
  }
  switch (addr)
  {
    case M_PSW: word = regs[PSW];
    break;
    case M_R7: word = regs[R7];
    break;
    case M_R6: word = regs[R6]; 
    break;
    case M_R5: word = regs[R5];
    break;
    case M_R4: word = regs[R4];
    break;
    case M_R3: word = regs[R3];
    break;
    case M_R2: word = regs[R2];
    break;
    case M_R1: word = regs[R1];
    break;
    case M_R0: word = regs[R0];
    break;    
    case O_DATA: IOgetbyte(out, lo);
                  word = lo;
    break;
    case O_STATUS:  word = Ostatusreg;
                    word = word & 0xFF00;
                    IOsendstatus(out, lo);
                    word += lo;
    break;
    case I_STATUS:  word = Istatusreg;
                    word = word & 0xFF00;
                    IOgetstatus(in, lo);
                    word += lo;
    break;
    case CLOCK: word = clkreg;
    break;
  }
  return false;
}

bool CPUreadbyte(u16 addr, u8 &byte)
{
  if (addr < 57344)
  {
    byte = ram[addr];
    return false;
  }
  switch (addr)
  {
    case M_PSW: byte = (u8)regs[PSW];
    break;
    case M_R7: byte = (u8)regs[PC];
    break;
    case M_R6: byte = (u8)regs[SP];
    break;
    case M_R5: byte = (u8)regs[R5];
    break;
    case M_R4: byte = (u8)regs[R4];
    break;
    case M_R3: byte = (u8)regs[R3];
    break;
    case M_R2: byte = (u8)regs[R2];
    break;
    case M_R1: byte = (u8)regs[R1];
    break;
    case M_R0: byte = (u8)regs[R0];
    break;    
    case I_DATA: IOgetbyte(in, byte);
    break;
    case O_STATUS: IOsendstatus(out, byte);
    break;
    case I_STATUS: IOgetstatus(in, byte);
    break;
    case CLOCK: byte = (u8)clkreg;
    break;
  }
  return false;  
}

bool CPUwriteword(u16 addr, u16 word)
{
  u8 hi;
  u8 lo;

  if (addr % 2 == 0)
  {
    if (addr < 57344)
    {
      lo = word & 0xFF;
      hi = word >> BYTE_SZ;

      ram[addr+1] = lo;
      ram[addr] = hi;

      return false;
    }
  }
  switch (addr)
  {
    case M_PSW: regs[PSW] = word;
    break;
    case M_R7: regs[PC] = word;
    break;
    case M_R6: regs[SP] = word;
    break;
    case M_R5: regs[R5] = word;
    break;
    case M_R4: regs[R4] = word;
    break;
    case M_R3: regs[R3] = word;
    break;
    case M_R2: regs[R2] = word;
    break;
    case M_R1: regs[R1] = word;
    break;
    case M_R0: regs[R0] = word;
    break;    
    case O_DATA:  lo = word & 0x00FF;
                  IOsendbyte(out, lo);
    break;
    case O_STATUS: Ostatusreg = word;
    break;
    case I_STATUS: Istatusreg = word;
    break;
    case CLOCK: clkreg = word;
    break;
  }

  return false;
}

bool CPUwritebyte(u16 addr, u8 byte)
{
  if (addr < 57344)
  {
    ram[addr] = byte;
    return false;
  }
  switch (addr)
  {
    case M_PSW: regs[PSW] &= 0xFF00; regs[PSW] += byte;                
    break;
    case M_R7: regs[PC] &= 0xFF00; regs[PC] += byte;
    break;
    case M_R6: regs[SP] &= 0xFF00; regs[SP] += byte;
    break;
    case M_R5: regs[R5] &= 0xFF00; regs[R5] += byte;
    break;
    case M_R4: regs[R4] &= 0xFF00; regs[R4] += byte;
    break;
    case M_R3: regs[R3] &= 0xFF00; regs[R3] += byte;
    break;
    case M_R2: regs[R2] &= 0xFF00; regs[R2] += byte;
    break;
    case M_R1: regs[R1] &= 0xFF00; regs[R1] += byte;
    break;
    case M_R0: regs[R0] &= 0xFF00; regs[R0] += byte;
    break;    
    case O_DATA: IOsendbyte(out, byte);
    break;
    case I_STATUS: Istatusreg = byte;
    break;
    case O_STATUS: Ostatusreg = byte;
    break;
    case CLOCK: clkreg = (u16)byte;
    break;
  }
  return false;
}

bool CPUreadreg(u16 reg, u16 &data)
{
  if (reg < NUMREGS)
  {
    data = regs[reg];
    return false;
  }
  return true;
}

bool CPUwritereg(u16 reg, u16 data)
{
  if (reg < NUMREGS)
  {
    regs[reg] = data;
    return false;
  }
  return true;
}

bool CPUsetflag(u8 flag, bool val)
{
  int bit;

  if (flag < 16)
  {
    bit = 1;
    bit = bit << flag;  

    if (val)
    {
      regs[PSW] = regs[PSW] | bit;
    }
    else
    {
      regs[PSW] = regs[PSW] & ~bit;
    }
    return false;
  }
  return true;
}

bool CPUtstflag(u8 flag, bool &val)
{
  int bit;

  if (flag < 16)
  {
    bit = 1;
    bit = bit << flag;

    if (regs[PSW] & bit)
    {
      val = true;
    }
    else
    {
      val = false;
    }

    return true;
  }
  return true;
}

bool CPUsetbreak(u16 addr)
{
  int cnt;

  for (cnt = 0; cnt < MAXBRK; cnt++)
  {
    if (brkpnts[cnt].enable == false)
    {
      brkpnts[cnt].enable = true;
      brkpnts[cnt].addr = addr;
      return false;
    }
  }
  return true;  
}

bool CPUclrbreak(u16 addr)
{
  int cnt;

  for (cnt = 0; cnt < MAXBRK; cnt++)
  {
    if (addr == brkpnts[cnt].addr && 
        brkpnts[cnt].enable == true)
    {
      brkpnts[cnt].enable = false;
      return false;
    }
  }
  return true;
}

bool CPUgo(s16 seconds, s16 instructions)
{
  u8 iostat;

  u16 val;

  int cnt;
  int instr;
  int newpri;

  time_t start;
  time_t now;

  time(&start);

  cnt = 0;
  thistick = 0;
  intswaiting = 0;
  instr = (int)instructions;

  run = 1;
  while (run)
  {
    if (clocktick == 1)
    {
      clocktick = 0;
      thistick += IPERTICK;
    }

    if (clocktick == -1 || thistick > 0)
    {
      if (CPUstep()) return false;
      thistick--;

      instr--;
      if (instr == 0)
      {
        return false;
      }
      if (instr % 5000 == 0 && seconds > 0)
      {
        time(&now);
  
        if (difftime(now, start) > seconds)
        {
          return false;
        }
      }
      if (instr % 5000 == 0)
      {
        IOgetstatus(in, iostat);

        if (iostat & IO_RDY)
        {
          CPUinterrupt(0x30);
        }
      }    

      for (cnt = 0; cnt < MAXBRK; cnt++)
      {
        if (regs[PC] == brkpnts[cnt].addr && brkpnts[cnt].enable == true)
        {
          return false;
        }
      }
    }
    if (intswaiting > 0)
    {
      for (cnt = 0; cnt < intswaiting; cnt++)
      {
        newpri = (regs[PSW] & 0xE0) >> 5;
        if (newpri >= intlist[cnt].priority)
        {
          INTpush(regs[PSW]);
          INTpush(regs[PC]);

          CPUreadword(intlist[cnt].vector, val);
          regs[PC] = val;
          CPUreadword(intlist[cnt].vector + 2, val);
          regs[PSW] = val;
          
          intlist[cnt].vector = intlist[intswaiting].vector;
          intlist[cnt].priority = intlist[intswaiting].vector;

          intswaiting--;

          break;
        }
      }
    }
  }
  return false;
}

bool CPUstep(void)
{
  WORD instr;

  u8 byteMode;
  u8 offset;

  u16 op;
  u16 src;
  u16 dst;

  CPUreadword(regs[PC], op);

  if (op == 0)
  {
    regs[PC] += 2;
    return true;
  }

  /* decode the instruction */
  INTu16_to_word(op, instr);

  /* Decode any wierd : jump & subroutine */
  if ((op & 0xFFC0) == 0x0040)
  {
    src = INTgetaddr(0, instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);
    OP_jmp(src);
    return false;
  }
  else if ((op & 0xFE00) == 0x0800)
  {
    src = INTgetaddr(0, 0, 0, 0, instr[8], instr[7], instr[6]);
    dst = INTgetaddr(0, instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);

    OP_jsr(src, dst);
    return false;
  }
  else if ((op & 0xFFF8) == 0x0080)
  {
    src = INTgetaddr(0, 0, 0, 0, instr[2], instr[1], instr[0]);
    OP_rts(src);
    return false;
  }
  else if ((op & 0xFFC0) == 0x0D00)
  {
    OP_mark(0x003F & op);
    return false;
  }
  else if ((op & 0xFE00) == 0x7E00)
  {
    src = INTgetaddr(0, 0, 0, 0, instr[8], instr[7], instr[6]);
    op = op & 0x1F;
    op = (~op) + 1;
    OP_sob(src, op);    
    return false;
  }

  /* Decode any wierd : Trap */
  if ((op & 0xFF00) == 0x8800)
  {
    OP_emt();
    return false;
  }
  else if ((op & 0xFF00) == 0x8900)
  {
    OP_trap();
    return false;
  }
  else if (op == 0x0003)
  {
    OP_bpt();
    return false;
  }
  else if (op == 0x0004)
  {
    OP_iot();
    return false;
  }
  else if (op == 0x0002)
  {
    OP_rti();
    return false;
  }
  else if (op == 0x0006)
  {
    OP_rtt();
    return false;
  }

  /* Decode any wierd : Misc */
  else if (op == 0x0001)
  {
    OP_wait();

    regs[PC] += 2;
    return false;
  }
  else if (op == 0x0005)
  {
    OP_reset();

    regs[PC] += 2;
    return false;
  }
  /* Decode any wierd : Condition Code  */
  else if ((op & 0xFFE0) == 0x00A0)
  {
    OP_cc(0x001F & op);

    regs[PC] += 2;
    return false;
  }

  /* Single Operand Instructions */
  if (instr[14] == 0 && instr[13] == 0 && instr[12] == 0)
  {
    byteMode = instr[15];
      
    /* Branch Instructions */
    if (instr[11] == 0)
    {
      offset = op & 0x00FF;

      if (instr[10] == 0 && instr[9] == 0 && instr[8] == 0)
      {
        if (byteMode)
        {
          OP_bpl(offset);
        }
        else if (instr[7] == 1 && instr[6] == 1)
        {
          dst = INTgetaddr(instr[15], instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);
          OP_swab(dst);
          regs[PC] += 2;
          return false;
        }

      }
      else if (instr[10] == 0 && instr[9] == 0 && instr[8] == 1)
      {
        byteMode ? OP_bmi(offset) : OP_br(offset);
      }
      else if (instr[10] == 0 && instr[9] == 1 && instr[8] == 0)
      {
        byteMode ? OP_bhi(offset) : OP_bne(offset);
      }
      else if (instr[10] == 0 && instr[9] == 1 && instr[8] == 1)
      {
        byteMode ? OP_blos(offset) : OP_beq(offset);
      }      
      else if (instr[10] == 1 && instr[9] == 0 && instr[8] == 0)
      {
        byteMode ? OP_bvc(offset) : OP_bge(offset);
      }
      else if (instr[10] == 1 && instr[9] == 0 && instr[8] == 1)
      {
        byteMode ? OP_bvs(offset) : OP_blt(offset);
      }
      else if (instr[10] == 1 && instr[9] == 1 && instr[8] == 0)
      {
        byteMode ? OP_bcc(offset) : OP_bgt(offset);
      }
      else if (instr[10] == 1 && instr[9] == 1 && instr[8] == 1)
      {
        byteMode ? OP_bcs(offset) : OP_blt(offset);
      }
      return false;
    }

    dst = INTgetaddr(instr[15], instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);

    if (instr[11] == 1 && instr[10] == 0 && instr[9] == 1)
    {
      /* OP CODE: CLR */
      if (instr[8] == 0 && instr[7] == 0 && instr[6] == 0)
      {        
        byteMode ? OP_clrb(dst) : OP_clr(dst);     
      }
      /* OP CODE: COM */
      else if (instr[8] == 0 && instr[7] == 0 && instr[6] == 1)
      {        
        byteMode ? OP_comb(dst) : OP_com(dst);     
      }
      /* OP CODE: INC */
      else if (instr[8] == 0 && instr[7] == 1 && instr[6] == 0)
      {
        byteMode ? OP_incb(dst) : OP_inc(dst);       
      }
      /* OP CODE: DEC */
      else if (instr[8] == 0 && instr[7] == 1 && instr[6] == 1)
      {
        byteMode ? OP_decb(dst) : OP_dec(dst);       
      }
      /* OP CODE: NEG */
      else if (instr[8] == 1 && instr[7] == 0 && instr[6] == 0)
      {
        byteMode ? OP_negb(dst) : OP_neg(dst);       
      }
      /* OP CODE: ADC */
      else if (instr[8] == 1 && instr[7] == 0 && instr[6] == 1)
      {
        byteMode ? OP_adcb(dst) : OP_adc(dst);
      }
      /* OP CODE: TST */
      else if (instr[8] == 1 && instr[7] == 1 && instr[6] == 1)
      {
        byteMode ? OP_tstb(dst) : OP_tst(dst);
      }

    }
    if (instr[11] == 1 && instr[10] == 1 && instr[9] == 0)
    {
      /* OP CODE: ASR */
      if (instr[8] == 0 && instr[7] == 1 && instr[6] == 0)
      {
        byteMode ? OP_asrb(dst) : OP_asr(dst);
      }
      /* OP CODE: ASL */
      else if (instr[8] == 0 && instr[7] == 1 && instr[6] == 1)
      {
        byteMode ? OP_aslb(dst) : OP_asl(dst);
      }
      /* OP CODE: ROR */
      else if (instr[8] == 0 && instr[7] == 0 && instr[6] == 0)
      {
        byteMode ? OP_rorb(dst) : OP_ror(dst);
      }
      /* OP CODE: ROL */
      else if (instr[8] == 0 && instr[7] == 0 && instr[6] == 1)
      {
        byteMode ? OP_rolb(dst) : OP_rol(dst);
      }

    }
    regs[PC] += 2;
    return false;
  }
  else
  {
    byteMode = instr[15];

    /* OP CODE: MOV */
    if (instr[14] == 0 && instr[13] == 0 && instr[12] == 1)
    {
      src = INTgetaddr(instr[15], instr[11], instr[10], instr[9], instr[8], instr[7], instr[6]);
      dst = INTgetaddr(instr[15], instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);
 
      byteMode ? OP_movb(src, dst) : OP_mov(src, dst);
    }
    /* OP CODE: CMP */
    else if (instr[14] == 0 && instr[13] == 1 && instr[12] == 0)
    {
      src = INTgetaddr(instr[15], instr[11], instr[10], instr[9], instr[8], instr[7], instr[6]);
      dst = INTgetaddr(instr[15], instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);

      byteMode ? OP_cmpb(src, dst) : OP_cmp(src, dst);
    }
    /* OP CODE: BIT */
    else if (instr[14] == 0 && instr[13] == 1 && instr[12] == 1)
    {
      src = INTgetaddr(instr[15], instr[11], instr[10], instr[9], instr[8], instr[7], instr[6]);
      dst = INTgetaddr(instr[15], instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);

      byteMode ? OP_bitb(src, dst) : OP_bit(src, dst);
    }
    /* OP CODE: BIC */
    else if (instr[14] == 1 && instr[13] == 0 && instr[12] == 0)
    {
      src = INTgetaddr(instr[15], instr[11], instr[10], instr[9], instr[8], instr[7], instr[6]);
      dst = INTgetaddr(instr[15], instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);

      byteMode ? OP_bicb(src, dst) : OP_bic(src, dst);
    }
    /* OP CODE: SUB/ADD */
    else if (instr[14] == 1 && instr[13] == 1 && instr[12] == 0)
    {
      src = INTgetaddr(0, instr[11], instr[10], instr[9], instr[8], instr[7], instr[6]);
      dst = INTgetaddr(0, instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);

      byteMode ? OP_sub(src, dst) : OP_add(src, dst);
    }
    /* OTHER 2-OP INSTRUCTIONS */
    else if (instr[15] == 0 && instr[14] == 1 && instr[13] == 1 && instr[12] == 1)
    {
      src = INTgetaddr(0, 0, 0, 0, instr[8], instr[7], instr[6]);
      dst = INTgetaddr(0, instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);

      /* OP CODE: XOR */
      if (instr[11] == 1 && instr[10] == 0 && instr[9] == 0)
      {
        OP_xor(src, dst);
      }
      /* OP CODE: MUL */
      else if (instr[11] == 0 && instr[10] == 0 && instr[9] == 0)
      {
        OP_mul(src, dst);
      }
      /* OP CODE: DIV */
      else if (instr[11] == 0 && instr[10] == 0 && instr[9] == 1)
      {
        OP_div(src, dst);
      }
    }
    regs[PC] += 2;
    return false;
  }

  return false;
}

bool CPUinterrupt(u16 intnum)
{
  u16 newpsw;

  if (intswaiting == 20) return true;

  CPUreadword(intnum + 2, newpsw);

  intlist[intswaiting].vector = intnum;
  intlist[intswaiting].priority = (newpsw & 0xE0) >> 5;

  intswaiting++;

  return false;
}
