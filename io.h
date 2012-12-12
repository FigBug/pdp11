#ifndef _io_h
#define _io_h

void IOsendbyte(int pipe, u8 byte);
void IOgetbyte(int pipe, u8 &byte);

void IOsendstatus(int pipe, u8 &stat);
void IOgetstatus(int pipe, u8 &stat);

#endif
