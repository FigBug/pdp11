#ifndef _debugger_h
#define _debugger_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cpu.h"
#include "decomp.h"

#define MAX_S16    32768    // 2^15
#define MIN_S16    -32769    //-(2^15 +1)
#define MAX_U16    65535    // 2^16-1
#define MIN_U16    0

#define MAX_U8    256      //2^8
#define MIN_U8    0
#define MAX_S8    128      //2^7
#define MIN_S8    -129    //-(2^7 +1)

//#define STRINGTABLESZ  32768
#define PCSTARTADDR  0x200

#define MAX_BYTE  0xff

#define ADDRMODE_MSK  0x003F
#define JSR_MSK   0xFE00
#define MAX_LABEL_LEN 9

//ooohh! data hiding! this is good stuff -- I remembered!  Do I get extra marks?
struct list{
  char label[MAX_LABEL_LEN];
  u16 addr;
  struct list *next;
};

int ST_isLabel(char *string);

void ST_addSymbol(char *label, u16 addr);

void ST_makeSymbolTables(FILE *fp);

void ST_freeStringTable(void);

void ST_freeLabelTable(void);

char *ST_lookupString(u16 addr);

void ST_stringTableInit();

bool ST_lookupLabel(char *findlabel, u16 &findaddr);

void MS_printBinaryString(u16 value);

void MS_printfCodeLine(u16 pcaddr);

void MS_stringtolower(char *string);

int MS_stringishex(char *string);

int MS_stringisdigit(char *string);

int MS_isValidS16(char *string, char *base);

int MS_isValidU16(char *string, char *base);

int MS_isValidU8(char *string, char *base);

char MS_valueAndType(char *string, int &value);

int MS_lookupFlag(char *string, u8 &flag);

void UI_help(char *parameters[], int n);

void UI_setMem(char *parameter[], int n);

void UI_readMem(char *parameter[], int n);

void UI_setReg(char *parameter[], int n);

void UI_readReg(char *parameter[], int n);

void UI_pcReset(int n);

void UI_setFlg(char *parameter[], int n);

void UI_setBreak(char *parameter[], int n);

void UI_clrBreak(char *parameter[], int n);

#endif
