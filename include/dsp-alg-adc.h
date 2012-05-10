#ifndef DSP_ALG_ADC_H
#define DSP_ALG_ADC_H

#include <dsp-alg-types.h>

typedef struct _adc t_adc;

t_adc *dsp_alg_adc_init(size_t channel_id);
void dsp_alg_adc_free(t_adc *);
void dsp_alg_adc(size_t n, t_adc *st, t_num *out);

#endif


