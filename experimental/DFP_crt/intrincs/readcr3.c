/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */

#include <intrin.h>

/* Register sizes are different between 32/64 bit mode.
 * So we have to do this for _WIN64 and _WIN32 seperatly.
 */

#ifdef _WIN64
  unsigned __int64 __readcr3(void)
  {
      unsigned __int64 value;
      __asm__ __volatile__ (
          "mov %%cr3, %[value]" 
          : [value] "=q" (value));
      return value;
  }
#else
  unsigned __LONG32 __readcr3(void)
  {
      unsigned __LONG32 value;
      __asm__ __volatile__ (
          "mov %%cr3, %[value]" 
          : [value] "=q" (value));
      return value;
  }
#endif
