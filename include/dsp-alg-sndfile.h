#ifndef DSP_ALG_SNDFILE_H
#define DSP_ALG_SNDFILE_H

#include <dsp-alg-types.h>

/* write files */

typedef struct _sndfilew t_sndfilew;

t_sndfilew *dsp_alg_sndfilew_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames, int format);

void dsp_alg_sndfilew_free(t_sndfilew *st);

void dsp_alg_sndfilew(size_t n, t_sndfilew *st, 
    const t_num *ina, const t_num *inb);

t_sndfilew *dsp_alg_wav16w_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames);

t_sndfilew *dsp_alg_wav24w_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames);

t_sndfilew *dsp_alg_wav32w_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames);

t_sndfilew *dsp_alg_aiff16w_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames);

t_sndfilew *dsp_alg_aiff24w_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames);

t_sndfilew *dsp_alg_aiff32w_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames);

typedef struct _sndfiler t_sndfiler;

/* read files */

t_sndfiler *dsp_alg_sndfiler_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames);

void dsp_alg_sndfiler_free(t_sndfiler *st);

void dsp_alg_sndfiler(size_t n, t_sndfiler *st, t_num *outa, t_num *outb);

#endif


