#ifndef IODEV_H_INCLUDED
#define IODEV_H_INCLUDED

void iodump();
bool ioparse(FILE *infile);
struct IoDevice getIO();
void IoDoCycleWork();
struct IoDev GetIo();

#endif