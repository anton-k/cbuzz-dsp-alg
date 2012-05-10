#ifndef DSP_ALG_PHASOR_H
#define DSP_ALG_PHASOR_H

#include <stdlib.h>
#include "dsp-alg-types.h"
#include "time.h"

/* phasor */

typedef struct _phasor t_phasor;

t_phasor *dsp_alg_phasor_init(size_t sample_rate, t_num cur);
void dsp_alg_phasor_free(t_phasor *st);

void dsp_alg_phasor(size_t n, t_phasor *st, const t_num *in, t_num *out);

/* linear table */
typedef struct _tabr t_tabr;

t_tabr *dsp_alg_tabr_init(size_t leng);
void dsp_alg_tabr_free(t_tabr *st);

size_t dsp_alg_tabr_length(t_tabr *a);
t_num *dsp_alg_tabr_samples(t_tabr *a);

void dsp_alg_tabr(t_tabr *st, const t_num *in, t_num *out);


typedef struct _oscil t_oscil;

t_oscil *dsp_alg_oscil_init(
    size_t sample_rate, t_num phase, t_tabr *samples);

void dsp_alg_oscil_free(t_oscil *st);
void dsp_alg_oscil(size_t n, t_oscil *st, const t_num *in, t_num *out);

#endif
