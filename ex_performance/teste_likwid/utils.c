#define _POSIX_C_SOURCE 199309L // Descobrir para que serve isso
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#include "utils.h"


/*  Retorna tempo em milisegundos desde EPOCH

    Forma de uso:
 
    rtime_t tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/
rtime_t timestamp (void)
{
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
  return ( (rtime_t) tp.tv_sec*1.0e3 + (rtime_t) tp.tv_nsec*1.0e-6 );
}

/*  Retorna tempo em milisegundos desde EPOCH

    Forma de uso:
    rtime_t tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/

// rtime_t timestamp(void) {
//   struct timeval tp;
//   gettimeofday(&tp, NULL);
//   return ( (rtime_t) tp.tv_sec*1.0e3 + (rtime_t) tp.tv_usec*1.0e-3 );
// }
