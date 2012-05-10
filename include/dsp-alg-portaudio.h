#ifndef DSP_ALG_PORTAUDIO_H
#define DSP_ALG_PORTAUDIO_H

#include <dsp-alg-types.h>

size_t dsp_alg_device_channels(size_t device);

typedef struct _paw t_paw;

t_paw *dsp_alg_paw_init(size_t sample_rate, size_t block_size,
    size_t device, size_t num_of_frames);

void dsp_alg_paw_free(t_paw *);
void dsp_alg_paw(size_t n, t_paw *st, const t_num **ins);

size_t dsp_alg_paw_channels(t_paw *);

typedef struct _par t_par;

t_par *dsp_alg_par_init(size_t sample_rate, size_t block_size,
    size_t device, size_t num_of_frames);

void dsp_alg_par_free(t_par *);
void dsp_alg_par(size_t n, t_par *st, t_num **outs);

size_t dsp_alg_par_channels(t_par *);

#endif
