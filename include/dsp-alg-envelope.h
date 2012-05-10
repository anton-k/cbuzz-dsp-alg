#ifndef DSP_ALG_ENVELOPE_H
#define DSP_ALG_ENVELOPE_H

#include "dsp-alg-types.h"
#include "time.h"

void dsp_alg_hold(size_t n, t_num cur, t_num* out);
void dsp_alg_line(size_t n, t_num incr, t_num *cur, t_num *out);
void dsp_alg_expline(size_t n, t_num incr, t_num *cur, t_num *out);

/* segments */

typedef struct _linseg t_linseg;
typedef t_linseg t_expseg;

t_linseg *dsp_alg_linseg_init(size_t sample_rate, size_t block_size);
void dsp_alg_linseg_free(t_linseg *);

t_expseg *dsp_alg_expseg_init(size_t sample_rate, size_t block_size);
void dsp_alg_expseg_free(t_expseg *);

/* clears all scheduled messages and sets last value
   to the current value */
void dsp_alg_linseg_reset(t_linseg *st);
void dsp_alg_expseg_reset(t_expseg *st);

void dsp_alg_linseg_append(t_linseg *st, t_num dur, t_num last);
void dsp_alg_expseg_append(t_expseg *st, t_num dur, t_num last);

void dsp_alg_linseg(size_t n, t_linseg *st, t_num *out);
void dsp_alg_expseg(size_t n, t_expseg *st, t_num *out);

#endif
