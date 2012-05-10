#ifndef DSP_ALG_ANALOG_H
#define DSP_ALG_ANALOG_H

#include <dsp-alg-types.h>

typedef struct _wave t_wave;

void dsp_alg_wave_free(t_wave *st);

t_wave *dsp_alg_sine_init();
t_wave *dsp_alg_square_init();
t_wave *dsp_alg_triangle_init();
t_wave *dsp_alg_saw_init();

void dsp_alg_sine    (size_t n, t_wave *st, const t_num *in, t_num *out);
void dsp_alg_square  (size_t n, t_wave *st, const t_num *in, t_num *out);
void dsp_alg_triangle(size_t n, t_wave *st, const t_num *in, t_num *out);
void dsp_alg_saw     (size_t n, t_wave *st, const t_num *in, t_num *out);

/* sample and hold */
typedef struct _sah t_sah;

/* dt - transition time */
t_sah *dsp_alg_sah_init(t_num dt);
void dsp_alg_sah_free(t_sah *);
void dsp_alg_sah(size_t n, t_sah *st, t_num *out);
void dsp_alg_sah_value(t_sah *st, t_num val);

typedef struct _vcf t_vcf;

t_vcf *dsp_alg_vcf_init();
void dsp_alg_vcf_free(t_vcf *);

void dsp_alg_vcf(size_t n, t_vcf *st, 
    const t_num *ina, const t_num *inb, t_num *out);

typedef struct _vca t_vca;

t_vca *dsp_alg_vca_init();
void dsp_alg_vca_free(t_vca *);

void dsp_alg_vca(size_t n, t_vca *st, 
    const t_num *ina, const t_num *inb, t_num *out);

#endif


