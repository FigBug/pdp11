#include <unistd.h>
#include <sys/poll.h>

#include "cpu.h"
#include "cpu_idefs.h"
#include "cpu_intrl.h"

#include "io.h"

u16 Odatareg = 0;
u16 Ostatusreg = IO_RDY;

u16 Idatareg = 0;
u16 Istatusreg = 0;

void IOsendbyte(int pipe, u8 byte)
{
	write(pipe, &byte, 1);
	Odatareg = byte;

	if (IO_INT & Ostatusreg)
	{
		CPUinterrupt(0x34);
	}
}

void IOgetbyte(int pipe, u8 &byte)
{
  struct pollfd ready[1];
	
	ready[0].fd = pipe;
	ready[0].events = POLLIN;
	ready[0].revents = 0;

	poll(ready, 1, 0);

	if (ready[0].revents & POLLIN)
	{
		read(pipe, &byte, 1);
		Idatareg = byte;
	}
	else
	{
	  byte = (u8)Idatareg;
	}
}

void IOsendstatus(int pipe, u8 &stat)
{
	stat = (u8)(Ostatusreg & IO_RDY);
}

void IOgetstatus(int pipe, u8 &stat)
{
	struct pollfd ready[1];

	ready[0].fd = pipe;
	ready[0].events = POLLIN;
	ready[0].revents = 0;

	poll(ready, 1, 0);

	if (ready[0].revents & POLLIN)
	{
		stat = Istatusreg & (~IO_RDY);
		stat = stat | IO_RDY;
	}
	else
	{
		stat = Istatusreg & (~IO_RDY);
	}
}
