#ifndef DSP_ALG_MATH_H
#define DSP_ALG_MATH_H

#include <math.h>
#include "dsp-alg-types.h"

void dsp_alg_sin(size_t n, const t_num *in, t_num *out);
void dsp_alg_cos(size_t n, const t_num *in, t_num *out);
void dsp_alg_tan(size_t n, const t_num *in, t_num *out);
void dsp_alg_asin(size_t n, const t_num *in, t_num *out);
void dsp_alg_acos(size_t n, const t_num *in, t_num *out);
void dsp_alg_atan(size_t n, const t_num *in, t_num *out);
void dsp_alg_atan2(size_t n, const t_num *ina, const t_num *inb, t_num *out);
void dsp_alg_sinh(size_t n, const t_num *in, t_num *out);
void dsp_alg_cosh(size_t n, const t_num *in, t_num *out);
void dsp_alg_tanh(size_t n, const t_num *in, t_num *out);
void dsp_alg_exp(size_t n, const t_num *in, t_num *out);
void dsp_alg_log(size_t n, const t_num *in, t_num *out);
void dsp_alg_log10(size_t n, const t_num *in, t_num *out);
void dsp_alg_pow(size_t n, const t_num *ina, const t_num *inb, t_num *out);
void dsp_alg_sqrt(size_t n, const t_num *in, t_num *out);
void dsp_alg_ceil(size_t n, const t_num *in, t_num *out);
void dsp_alg_floor(size_t n, const t_num *in, t_num *out);
void dsp_alg_fabs(size_t n, const t_num *in, t_num *out);
void dsp_alg_fmod(size_t n, const t_num *ina, const t_num *inb, t_num *out);

#endif

