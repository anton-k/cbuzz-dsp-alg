#ifndef DSP_ALG_FILTER_H
#define DSP_ALG_FILTER_H

#include "dsp-alg-types.h"

typedef struct {
    const t_options *opt;
    t_num a0, a1, a2, b1, b2;   /* coefficients */
    t_num x1, x2, y1, y2;       /* delayed samples */
} t_biquad;

void dsp_alg_biquad_init(
    t_biquad *out, const t_options *opt,
    t_num a0, t_num a1, t_num a2,
    t_num b1, t_num b2);

void dsp_alg_biquad_set(
    t_biquad *st,
    t_num a0, t_num a1, t_num a2,
    t_num b1, t_num b2);

void dsp_alg_biquad(t_biquad *st, const t_num *in, t_num *out);

/* Butterworth filters */
/* f  -- central frequency
   bw -- bandwidth
*/

typedef struct {
    t_biquad biquad;
    t_num f;
    t_num bw;
} t_butter; 


void dsp_alg_butter(t_butter *st, const t_num *in, t_num *out);

void dsp_alg_lp_init(t_butter *res, const t_options *opt, t_num f);
void dsp_alg_hp_init(t_butter *res, const t_options *opt, t_num f);
void dsp_alg_bp_init(t_butter *res, const t_options *opt, t_num f, t_num bw);
void dsp_alg_br_init(t_butter *res, const t_options *opt, t_num f, t_num bw);

/* update coefficients */

/* f */

void dsp_alg_lp_set_f(t_butter *st, t_num f);
void dsp_alg_hp_set_f(t_butter *st, t_num f);
void dsp_alg_bp_set_f(t_butter *st, t_num f);
void dsp_alg_br_set_f(t_butter *st, t_num f);

/* bw */

void dsp_alg_bp_set_bw(t_butter *st, t_num bw);
void dsp_alg_br_set_bw(t_butter *st, t_num bw);

/* f && bw */

void dsp_alg_bp_set_f_bw(t_butter *st, t_num f, t_num bw);
void dsp_alg_br_set_f_bw(t_butter *st, t_num f, t_num bw);

#endif
