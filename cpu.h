#ifndef _pdp11_cpu
#define _pdp11_cpu

/* type defenitions (for a 32-bit machine) */
#define u8		unsigned char
#define u16		unsigned short int
#define u32		unsigned long int

#define s8		signed char
#define s16		signed short int
#define s32		signed long int

/* size of a character in bits */
#define BYTE_SZ 8

/* size of a word in bits */
#define WORD_SZ	16

/* bits per storage unit */
#define BPSU	  8

/* maximum break points */
#define MAXBRK  5

/* register names */
#define R0		0
#define R1		1
#define R2		2
#define R3		3
#define R4		4
#define R5		5
#define R6		6
#define SP    6
#define R7		7
#define PC    7
#define PSW		8
#define NUMREGS 9 

/* flag names */
#define F0		0
#define FC		0
#define F1		1
#define	FV		1
#define F2		2
#define FZ		2
#define F3		3
#define FN		3
#define F4		4
#define FT		4
#define F5		5
#define F6		6
#define F7		7
#define F8		8
#define F9		9
#define F10		10
#define F11		11
#define F12		12
#define F13		13
#define F14		14
#define F15		15

/* Use for setting/clearing flags */
#define clear 0
#define set   1

/* memory mapped I/O devices */

#define M_PSW 0xFFFE
#define M_PC  0xFFC7 
#define M_R7  0xFFC7 
#define M_SP  0xFFC6 
#define M_R6  0xFFC6 
#define M_R5  0xFFC5 
#define M_R4  0xFFC4 
#define M_R3  0xFFC3 
#define M_R2  0xFFC2 
#define M_R1  0xFFC1       
#define M_R0  0xFFC0 

#define O_DATA		0xFF76
#define O_STATUS	0xFF74
#define I_DATA		0xFF72
#define I_STATUS	0xFF70

#define CLOCK     0xFF66
/* Some IO stuff */

#define IO_INT 0x40
#define IO_RDY 0x80

/*
 * All functions return:
 *	- true		: error 
 *	- false		: no error
 */

/* Initilize the CPU: call before any other function */
bool CPUinit(void);

/* Shut down the CPU: call before exiting program */
bool CPUhalt(void);

/* Read from main memory or memory mapped device */ 
bool CPUreadword(u16 addr, u16 &word);

/* Read from main memory or memory mapped device */ 
bool CPUreadbyte(u16 addr, u8 &byte);

/* Write to main memory or memory mapped device */
bool CPUwriteword(u16 addr, u16 word);

/* Write to main memory or memory mapped device */
bool CPUwritebyte(u16 addr, u8 byte);

/* Read from a register */
bool CPUreadreg(u16 reg, u16 &data);

/* Write to a register */
bool CPUwritereg(u16 reg, u16 data);

/* set a PSW flag */
bool CPUsetflag(u8 flag, bool val);

/* test a PSW flag */
bool CPUtstflag(u8 flag, bool &val);

/* Set a break point at addr */
bool CPUsetbreak(u16 addr);

/* Remove a break point at addr */
bool CPUclrbreak(u16 addr);

/* 
 * Execute instructions for a max of num of seconds or 
 * num of instructions. 
 *
 * seconds < 0		: no max run time
 * instructions < 0 : no max instructions
 */
bool CPUgo(s16 seconds, s16 instructions);

/* 
 * Execute one instruction, returns: 
 *  - false:  instruction executed correctly
 *  - true:   executed HALT instruction or fatal error
 */
bool CPUstep(void);

/*
 *  Interrupt the CPU
 *   - to be used by threads emulating hardware devices
 */
bool CPUinterrupt(u16 intnum);

#endif
