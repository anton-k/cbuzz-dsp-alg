#ifndef DSP_ALG_NUM_H
#define DSP_ALG_NUM_H

#include "dsp-alg-types.h"

/* resampling */

void dsp_alg_resample(size_t n, t_num ratio, t_num *in, t_num *out);

/* ------------------------------------------------------------------ */
/* arithmetic */

void dsp_alg_bias(size_t n, t_num k, const t_num *in, t_num *out);
void dsp_alg_scale(size_t n, t_num k, const t_num *in, t_num *out);

void dsp_alg_add(size_t n, const t_num *ina, const t_num *inb, t_num *out);
void dsp_alg_sub(size_t n, const t_num *ina, const t_num *inb, t_num *out);
void dsp_alg_negate(size_t n, const t_num *in, t_num *out);
void dsp_alg_mul(size_t n, const t_num *ina, const t_num *inb, t_num *out);
void dsp_alg_div(size_t n, const t_num *ina, const t_num *inb, t_num *out);
void dsp_alg_recip(size_t n, const t_num *in, t_num *out);


/* ------------------------------------------------------------- */
/* logic */

void dsp_alg_true(size_t n, t_bool *out);
void dsp_alg_false(size_t n, t_bool *out);

void dsp_alg_not(size_t n, const t_bool *in, t_bool *out);
void dsp_alg_and(size_t n, const t_bool *ina, const t_bool *inb, t_bool *out);
void dsp_alg_or(size_t n, const t_bool *ina, const t_bool *inb, t_bool *out);
void dsp_alg_xor(size_t n, const t_bool *ina, const t_bool *inb, t_bool *out);

/* ------------------------------------------------------------- */
/* comparison */

void dsp_alg_eq(size_t n, const t_num *ina, const t_num *inb, t_bool *out);
void dsp_alg_neq(size_t n, const t_num *ina, const t_num *inb, t_bool *out);

void dsp_alg_lt(size_t n, const t_num *ina, const t_num *inb, t_bool *out);
void dsp_alg_gt(size_t n, const t_num *ina, const t_num *inb, t_bool *out);
void dsp_alg_lteq(size_t n, const t_num *ina, const t_num *inb, t_bool *out);
void dsp_alg_gteq(size_t n, const t_num *ina, const t_num *inb, t_bool *out);

/* inv :  out = 1 - in */
void dsp_alg_inv(size_t n, const t_num *in, t_num *out);
void dsp_alg_min(size_t n, const t_num *ina, const t_num *inb, t_num *out);
void dsp_alg_max(size_t n, const t_num *ina, const t_num *inb, t_num *out);

/* --------------------------------------------------- */
/* wrappers */

void dsp_alg_wrap(size_t n, const t_num *in, t_num *out);
void dsp_alg_clip(size_t n, const t_num *in, t_num *out);

#endif
