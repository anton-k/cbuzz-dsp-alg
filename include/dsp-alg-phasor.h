#ifndef DSP_ALG_PHASOR_H
#define DSP_ALG_PHASOR_H

#include <stdlib.h>
#include "dsp-alg-types.h"
#include "time.h"

/* phasor */

typedef struct {
    t_num cur;
    t_num recip_sr;
    t_num bs;
} t_phasor;

void dsp_alg_phasor_init(t_phasor *res, const t_options *opt, t_num cur);
void dsp_alg_phasor(t_phasor *st, const t_num *in, t_num *out);

/* linear table */
typedef struct {
    size_t leng;
    t_num *samples;
    t_num bs;
} t_tabr;

void dsp_alg_tabr_init(t_tabr *res, const t_options *opt, 
    size_t leng, t_num *samples);

size_t dsp_alg_tabr_length(t_tabr *a);
t_num *dsp_alg_tabr_samples(t_tabr *a);

void dsp_alg_tabr(t_tabr *st, const t_num *in, t_num *out);


typedef struct {
    t_phasor *phasor;
    t_tabr   *tabr;  
} t_oscil;

void dsp_alg_oscil_init(t_oscil *, const t_options *, 
    t_phasor *phasor, t_tabr *samples);
void dsp_alg_oscil(t_oscil *st, const t_num *in, t_num *out);

#endif
