#ifndef DSP_ALG_DAC_H
#define DSP_ALG_DAC_H

#include <dsp-alg-types.h>

typedef struct _dac t_dac;

t_dac *dsp_alg_dac_init(size_t channel_id);
void dsp_alg_dac_free(t_dac *);
void dsp_alg_dac(size_t n, t_dac *st, const t_num *in);

#endif


