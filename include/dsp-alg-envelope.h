#ifndef DSP_ALG_ENVELOPE_H
#define DSP_ALG_ENVELOPE_H

#include <dsp-alg-types.h>
#include <time.h>

/* low level segments */

/* There is assumption that breackpoints live in
	control rate, so each segment contains at least
	block_size of points, and "dur" is count in 
	control rate ticks !not sample rate! */
typedef struct{
	t_num incr;		/* increment */
	t_num base;		/* base point */
	t_time dur;		/* duration in control rate ticks */
    const t_options *opt; /* refrence to global options */
} t_seg;

/* linear segments */

typedef struct {
    t_seg seg;
} t_linseg;

void dsp_alg_linseg_init( t_linseg *res, const t_options *opt, double x0 );
void dsp_alg_linseg( t_linseg *st, t_num *out );
void dsp_alg_linseg_set( t_linseg *st, t_num dur, t_num next_val );

/* exponential segments */

typedef struct {
    t_seg seg;
} t_expseg;

void dsp_alg_expseg_init( t_expseg *res, const t_options *opt, double x0 );
void dsp_alg_expseg( t_expseg *st, t_num *out );
void dsp_alg_expseg_set( t_expseg *st, t_num dur, t_num next_val );

#endif
