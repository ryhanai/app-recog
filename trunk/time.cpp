// -*- C++ -*-
/*!
 * @file time.cpp
 * @date $Date$
 *
 * $Id$
 */

#include <stdio.h>

//#define USE_RDTSC

#ifndef USE_RDTSC

#include <sys/time.h>

unsigned long current_utime(void){
  struct timeval t;
  if (gettimeofday( &t, 0 ) == -1 )
    return 0;
  return t.tv_sec * 1000000 + t.tv_usec;
}

#else 

#define CPU_CLOCK 2160 /* 2160 MHz */

typedef union {
  unsigned long long val;
  struct {
    unsigned long eax;
    unsigned long edx;
  } reg;
} time_stamp_t;

#define rdtsc(tsc)\
__asm__ __volatile__("rdtsc"\
                    : "=a" (tsc.reg.eax), "=d" (tsc.reg.edx)\
                    )

static time_stamp_t now, start;
static void reset_utime(){ start.val = 0ULL; }

unsigned long current_utime(void){
    rdtsc(now);
    return (now.val - start.val) / CPU_CLOCK;
}

#endif

int tak (int x, int y, int z) {
  if (y >= x) {
    return z;
  } else {
    return tak(tak(x - 1, y, z), tak(y - 1, z, x), tak(z - 1, x, y));
  }
}

#ifdef WITH_MAIN

int main (int argc, char *argv[]) {

  unsigned long start, end;
#ifdef USE_RDTSC
  reset_utime();
#endif

  {
    int val, x, y, z;
    x = 30; y = 10; z = 5;
    start = current_utime();
    val = tak( x, y, z );
    // sleep (2);
    end = current_utime();
    printf ("tak(%d,%d,%d) took %lu [microsecs].\n", x, y, z, end - start);
  }

  return 0;
}

#endif


// echo | gcc -E -v -march=native - 2>&1 | sed '/march/!d;s/.*\(-march\)/\1/'
