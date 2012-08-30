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

typedef struct _options {
    size_t sr, cr, bs;
    t_num pi_sr, tau_sr, recip_sr;
    t_num pi, sqrt2, eps, tau;
} t_options;

void options_init( t_options *opt, 
    size_t sample_rate, size_t block_size );

t_num *sig_init(const t_options *);
void sig_free(t_num *a);

t_bool *sig_bool_init(const t_options *);
void sig_bool_free(t_bool *a);

void sig_print(const t_options *opt, const t_num *in);
#endif
