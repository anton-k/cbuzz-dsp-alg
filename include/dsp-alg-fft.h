#ifndef DSP_ALG_FFT_H
#define DSP_ALG_FFT_H

#include <dsp-alg-types.h>

typedef struct _fft t_fft;

t_fft *dsp_alg_fft_init(void);
void dsp_alg_fft_free(t_fft *);
void dsp_alg_fft(size_t n, t_fft *st, const t_num *in);
void dsp_alg_ifft(size_t n, t_fft *st, t_num *out);

#endif


