#ifndef DSP_TYPES_H
#define DSP_TYPES_H

#include <stdbool.h>
#include <stdlib.h>

/*  constant 2 * PI   */
#ifndef TAU
#define TAU 6.283185307179586
#endif

#ifndef PI
#define PI  3.141592653589793
#endif

#ifndef SQRT2
#define SQRT2 1.4142135623730951
#endif

#ifndef EPS
#define EPS 1e-6
#endif


typedef double t_num;
typedef bool   t_bool;

t_num pi_sr(size_t sample_rate);
t_num tau_sr(size_t sample_rate);

size_t control_rate(size_t sample_rate, size_t block_size);

t_num *sig_init(size_t block_size);
void sig_free(t_num *a);

void sig_print(size_t n, const t_num *in);
#endif
