#ifndef __UTILS_H__
#define __UTILS_H__


#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

typedef double rtime_t;

#define ALIGN __attribute__((aligned)) 
#define ALIGN_64 __attribute__((aligned(64)))
#define ALIGN_32 __attribute__((aligned(32)))
#define ALIGN_16 __attribute__((aligned(16)))
#define ALIGN_8 __attribute__((aligned(8)))


rtime_t timestamp(void);

#endif // __UTILS_H__