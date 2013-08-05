/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */

#include <intrin.h>

void __outword(unsigned short Port,unsigned short Data)
{
    __asm__ __volatile__ ("outw %w0,%w1"
        :
    : "a" (Data), "Nd" (Port));
}
