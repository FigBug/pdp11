#ifndef _pdp11_cpu_idefs
#define _pdp11_cpu_idefs

#include "cpu.h"

/* Usefull #defines */
#define KB		1024
#define MEM_SZ	28

/* WORD: Array of 16 bits */
typedef u8 WORD[WORD_SZ];

/* BYTE: Array of 8 bits */
typedef u8 BYTE[BYTE_SZ];

/* Interrupt: enable flag & address */
typedef struct {
	bool	enable;
	u16		addr;
} BREAK_PNT;

#define IPERTICK 33333

#endif
