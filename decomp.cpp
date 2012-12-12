#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "cpu_intrl.h"
#include "cpu_idefs.h"

#include "decomp.h"

char *DECregstr(int b2, int b1, int b0)
{
  char *rstr;

  if (b2 == 0 && b1 == 0 && b0 == 0) rstr = "r0";
  if (b2 == 0 && b1 == 0 && b0 == 1) rstr = "r1";
  if (b2 == 0 && b1 == 1 && b0 == 0) rstr = "r2";
  if (b2 == 0 && b1 == 1 && b0 == 1) rstr = "r3";
  if (b2 == 1 && b1 == 0 && b0 == 0) rstr = "r4";
  if (b2 == 1 && b1 == 0 && b0 == 1) rstr = "r5";
  if (b2 == 1 && b1 == 1 && b0 == 0) rstr = "sp";
  if (b2 == 1 && b1 == 1 && b0 == 1) rstr = "pc";

  return rstr;
}

char *DECgetaddr(u16 addr, int bm, int b5, int b4, int b3, int b2, int b1, int b0)
{
  static char regstr[16];

  u16 nxtwrd;

  regstr[0] = 0;

  CPUreadword(addr + 2, nxtwrd);

  if (b2 == 1 && b1 == 1 && b0 == 1)
  {
    if (b5 == 0 && b4 == 1 && b3 == 0)
      sprintf(regstr, "#%4.4X", nxtwrd);
    if (b5 == 0 && b4 == 1 && b3 == 1)
      sprintf(regstr, "@#%4.4X", nxtwrd);
    if (b5 == 1 && b4 == 1 && b3 == 0)
      sprintf(regstr, "%4.4X", nxtwrd + 4 + addr);
    if (b5 == 1 && b4 == 1 && b3 == 1)
      sprintf(regstr, "@%4.4X", nxtwrd + 4 + addr);
    if (regstr[0] != 0)
      return regstr;
  }

  if (b5 == 0 && b4 == 0 && b3 == 0)
    sprintf(regstr, "%s", DECregstr(b2, b1, b0)); 
  if (b5 == 0 && b4 == 0 && b3 == 1) 
    sprintf(regstr, "(%s)", DECregstr(b2, b1, b0)); 
  if (b5 == 0 && b4 == 1 && b3 == 0) 
    sprintf(regstr, "(%s)+", DECregstr(b2, b1, b0)); 
  if (b5 == 0 && b4 == 1 && b3 == 1) 
    sprintf(regstr, "@(%s)+", DECregstr(b2, b1, b0)); 
  if (b5 == 1 && b4 == 0 && b3 == 0) 
    sprintf(regstr, "-(%s)", DECregstr(b2, b1, b0)); 
  if (b5 == 1 && b4 == 0 && b3 == 1) 
    sprintf(regstr, "@-(%s)", DECregstr(b2, b1, b0)); 
  if (b5 == 1 && b4 == 1 && b3 == 0) 
    sprintf(regstr, "%4.4X(%s)", nxtwrd, DECregstr(b2, b1, b0)); 
  if (b5 == 1 && b4 == 1 && b3 == 1)
    sprintf(regstr, "%4.4X(%s)", nxtwrd, DECregstr(b2, b1, b0)); 

  return regstr;
}

bool DECdecompile(u16 addr, char *text)
{
  WORD instr;

  u8 byteMode;
  u8 offset;

  u16 op;

  char *src;
  char *dst;
  char *buf;

  CPUreadword(addr, op);

  text[0] = 0;

  if (op == 0)
  {
    sprintf(text, "halt");
    return false;
  }

  /* decode the instruction */
  INTu16_to_word(op, instr);

  /* Decode any wierd : jump & subroutine */
  if (op & 0xFFC0 == 0x0040)
  {
    src = DECgetaddr(addr, 0, instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);
    sprintf(text, "jmp %s", src); 
    return false;
  }
  else if (op & 0xFE00 == 0x0800)
  {
    src = DECgetaddr(addr, 0, 0, 0, 0, instr[8], instr[7], instr[6]);
    buf = strdup(src);
    dst = DECgetaddr(addr, 0, instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);

    sprintf(text, "jsr %s, %s", buf, dst);
    free(buf);
    return false;
  }
  else if (op & 0xFFF8 == 0x0080)
  {
    src = DECgetaddr(addr, 0, 0, 0, 0, instr[2], instr[1], instr[0]);
    sprintf(text, "rts %s", src);
    return false;
  }
  else if (op & 0xFFC0 == 0x0D00)
  {
    sprintf(text, "mark %2.2X", (int)(op & 0x003F));
    return false;
  }
  else if (op & 0xFE00 == 0x7E00)
  {
    src = DECgetaddr(addr, 0, 0, 0, 0, instr[8], instr[7], instr[6]);
    sprintf(text, "sob %s, %2.2X", src, (int)(0x003F & op));
    return false;
  }

  /* Decode any wierd : Trap */
  if (op & 0xFF00 == 0x8800)
  {
    sprintf(text, "emt");
    return false;
  }
  else if (op & 0xFF00 == 0x8900)
  {
    sprintf(text, "trap");
    return false;
  }
  else if (op == 0x0003)
  {
    sprintf(text, "bpt");
    return false;
  }
  else if (op == 0x0004)
  {
    sprintf(text, "iot");
    return false;
  }
  else if (op == 0x0002)
  {
    sprintf(text, "rti");
    return false;
  }
  else if (op == 0x0006)
  {
    sprintf(text, "rtt");
    return false;
  }

  /* Decode any wierd : Misc */
  else if (op == 0x0001)
  {
    sprintf(text, "wait");
    return false;
  }
  else if (op == 0x0005)
  {
    sprintf(text, "reset");
    return false;
  }
  /* Decode any wierd : Condition Code  */
  else if (op & 0xFFE0 == 0x00A0)
  {
    sprintf(text, "cc %2.2X", (int)(0x001F &op));
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
          sprintf(text, "bpl %2.2X", offset);
        }
        else if (instr[7] == 1 && instr[6] == 1)
        {
          dst = DECgetaddr(addr, instr[15], instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);
          sprintf(text, "swab %s", dst);
          return false;
        }
      }
      else if (instr[10] == 0 && instr[9] == 0 && instr[8] == 1)
      {
        sprintf(text, "%s %2.2X", byteMode ? "bmi" : "br", offset);
      }
      else if (instr[10] == 0 && instr[9] == 1 && instr[8] == 0)
      {
        sprintf(text, "%s %2.2X", byteMode ? "bhi" : "bne", offset);        
      }
      else if (instr[10] == 0 && instr[9] == 1 && instr[8] == 1)
      {
        sprintf(text, "%s %2.2X", byteMode ? "blos" : "beq", offset);        
      }      
      else if (instr[10] == 1 && instr[9] == 0 && instr[8] == 0)
      {
        sprintf(text, "%s %2.2X", byteMode ? "bvc" : "bge", offset);        
      }
      else if (instr[10] == 1 && instr[9] == 0 && instr[8] == 1)
      {
        sprintf(text, "%s %2.2X", byteMode ? "bvs" : "blt", offset);        
      }
      else if (instr[10] == 1 && instr[9] == 1 && instr[8] == 0)
      {
        sprintf(text, "%s %2.2X", byteMode ? "bcc" : "bgt", offset);        
      }
      else if (instr[10] == 1 && instr[9] == 1 && instr[8] == 1)
      {
        sprintf(text, "%s %2.2X", byteMode ? "bcs" : "blt", offset);        
      }
      return false;
    }

    dst = DECgetaddr(addr, instr[15], instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);

    if (instr[11] == 1 && instr[10] == 0 && instr[9] == 1)
    {
      /* OP CODE: CLR */
      if (instr[8] == 0 && instr[7] == 0 && instr[6] == 0)
      {        
        sprintf(text, "%s %s", byteMode ? "clrb" : "clr", dst);
      }
      /* OP CODE: COM */
      else if (instr[8] == 0 && instr[7] == 0 && instr[6] == 1)
      {        
        sprintf(text, "%s %s", byteMode ? "comb" : "com", dst);
      }
      /* OP CODE: INC */
      else if (instr[8] == 0 && instr[7] == 1 && instr[6] == 0)
      {
        sprintf(text, "%s %s", byteMode ? "incb" : "inc", dst);
      }
      /* OP CODE: DEC */
      else if (instr[8] == 0 && instr[7] == 1 && instr[6] == 1)
      {
        sprintf(text, "%s %s", byteMode ? "decb" : "dec", dst);
      }
      /* OP CODE: NEG */
      else if (instr[8] == 1 && instr[7] == 0 && instr[6] == 0)
      {
        sprintf(text, "%s %s", byteMode ? "negb" : "neg", dst);
      }
      else if (instr[8] == 1 && instr[7] == 0 && instr[6] == 1)
      {
        sprintf(text, "%s %s", byteMode ? "adcb" : "adc", dst);
      }
      /* OP CODE: TST */
      else if (instr[8] == 1 && instr[7] == 1 && instr[6] == 1)
      {
        sprintf(text, "%s %s", byteMode ? "tstb" : "tst", dst);
      }

    }
    if (instr[11] == 1 && instr[10] == 1 && instr[9] == 0)
    {
      /* OP CODE: ASR */
      if (instr[8] == 0 && instr[7] == 1 && instr[6] == 0)
      {
        sprintf(text, "%s %s", byteMode ? "asrb" : "asr", dst);
      }
      /* OP CODE: ASL */
      else if (instr[8] == 0 && instr[7] == 1 && instr[6] == 1)
      {
        sprintf(text, "%s %s", byteMode ? "aslb" : "asl", dst);
      }
      /* OP CODE: ROR */
      else if (instr[8] == 0 && instr[7] == 0 && instr[6] == 0)
      {
        sprintf(text, "%s %s", byteMode ? "rorb" : "ror", dst);
      }
      /* OP CODE: ROL */
      else if (instr[8] == 0 && instr[7] == 0 && instr[6] == 1)
      {
        sprintf(text, "%s %s", byteMode ? "rolb" : "rol", dst);
      }

    }
  }
  else
  {
    byteMode = instr[15];
    buf  = DECgetaddr(addr, instr[15], instr[5], instr[4], instr[3], instr[2], instr[1], instr[0]);
    dst = strdup(buf);
    src = DECgetaddr(addr, instr[15], instr[11], instr[10], instr[9], instr[8], instr[7], instr[6]);

    /* OP CODE: MOV */
    if (instr[14] == 0 && instr[13] == 0 && instr[12] == 1)
    {
      sprintf(text, "%s %s, %s", byteMode ? "movb" : "mov", src, dst);
    }
    /* OP CODE: CMP */
    else if (instr[14] == 0 && instr[13] == 1 && instr[12] == 0)
    {
      sprintf(text, "%s %s, %s", byteMode ? "cmpb" : "cmp", src, dst);
    }
    /* OP CODE: BIT */
    else if (instr[14] == 0 && instr[13] == 1 && instr[12] == 1)
    {
      sprintf(text, "%s %s, %s", byteMode ? "bitb" : "bit", src, dst);
    }
    /* OP CODE: BIC */
    else if (instr[14] == 1 && instr[13] == 0 && instr[12] == 0)
    {
      sprintf(text, "%s %s, %s", byteMode ? "bicb" : "bic", src, dst);
    }
    /* OP CODE: SUB/ADD */
    else if (instr[14] == 1 && instr[13] == 1 && instr[12] == 0)
    {
      sprintf(text, "%s %s, %s", byteMode ? "sub" : "add", src, dst);
    }
    /* OTHER 2-OP INSTRUCTIONS */
    else if (instr[15] == 0 && instr[14] == 1 && instr[13] == 1 && instr[12] == 1)
    {
      src = DECgetaddr(addr, 0, 0, 0, 0, instr[8], instr[7], instr[6]);
      /* OP CODE: XOR */
      if (instr[11] == 1 && instr[10] == 0 && instr[9] == 0)
      {
        sprintf(text, "xor %s %s", src, dst);
      }
      /* OP CODE: MUL */
      else if (instr[11] == 0 && instr[10] == 0 && instr[9] == 0)
      {
        sprintf(text, "mul %s %s", src, dst);
      }
      /* OP CODE: DIV */
      else if (instr[11] == 0 && instr[10] == 0 && instr[9] == 1)
      {
        sprintf(text, "div %s %s", src, dst);
      }
    }
  }
  return false;
}
