#
# Makefile: PDP-11 Emulator
#
OBJS=main.o cpu.o instr_do.o instr_so.o instr_ms.o instr_pc.o cpu_intrl.o tables.o misc.o help.o setandread.o io.o decomp.o
CFLAGS=-g -Wall
CC=g++

# all
all: pdp11 pdpclk pdptrm assem

# The Assembler
assem: assem.c
	$(CC) $(CFLAGS) assem.c -o assem

# The main program
pdp11: $(OBJS)
	$(CC) $(CFLAGS) -o pdp11 $(OBJS)

# The Debugger
main.o: main.cpp cpu.h debugger.h
	$(CC) $(CFLAGS) -c main.cpp

tables.o: tables.cpp debugger.h
	$(CC) $(CFLAGS) -c tables.cpp

help.o: help.cpp debugger.h
	$(CC) $(CFLAGS) -c help.cpp

setandread.o: setandread.cpp debugger.h
	$(CC) $(CFLAGS) -c setandread.cpp

misc.o: misc.cpp debugger.h
	$(CC) $(CFLAGS) -c misc.cpp

decomp.o: decomp.cpp cpu.h cpu_idefs.h
	$(CC) $(CFLAGS) -c decomp.cpp 

# The Emulator
cpu.o: cpu.cpp cpu.h cpu_idefs.h cpu_intrl.cpp
	$(CC) $(CFLAGS) -c cpu.cpp

cpu_intrl.o: cpu_intrl.cpp cpu_intrl.h
	$(CC) $(CFLAGS) -c cpu_intrl.cpp

instr_do.o: instr_do.cpp instr_do.h
	$(CC) $(CFLAGS) -c instr_do.cpp

instr_so.o: instr_so.cpp instr_so.h
	$(CC) $(CFLAGS) -c instr_so.cpp

instr_pc.o: instr_pc.cpp instr_pc.h
	$(CC) $(CFLAGS) -c instr_pc.cpp

instr_ms.o: instr_ms.cpp instr_ms.h
	$(CC) $(CFLAGS) -c instr_ms.cpp

io.o: io.cpp io.h
	$(CC) $(CFLAGS) -c io.cpp

# the PDP-11 Clock Emularor
pdpclk: pdpclk.cpp
	$(CC) $(CFLAGS) -o pdpclk pdpclk.cpp

# the PDP-11 Terminal Emulator
pdptrm: pdptrm.cpp
	$(CC) $(CFLAGS) -o pdptrm pdptrm.cpp
