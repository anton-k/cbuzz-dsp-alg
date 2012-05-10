#ifndef DSP_ALG_FILTER_H
#define DSP_ALG_FILTER_H

#include "dsp-alg-types.h"

typedef struct _biquad t_biquad;

t_biquad* dsp_alg_biquad_init(
    t_num a0, t_num a1, t_num a2,
    t_num b1, t_num b2);

void dsp_alg_biquad_free(t_biquad *in); 
void dsp_alg_biquad(size_t n, t_biquad *st, const t_num *in, t_num *out);


/* Butterworth filters */
/* f  -- central frequency
   bw -- bandwidth
*/

typedef struct _butter t_butter; 

void dsp_alg_butter_free(t_butter *st);
void dsp_alg_butter(size_t n, t_butter *st, const t_num *in, t_num *out);

t_butter *dsp_alg_lp_init(size_t sample_rate, t_num f);
t_butter *dsp_alg_hp_init(size_t sample_rate, t_num f);
t_butter *dsp_alg_bp_init(size_t sample_rate, t_num f, t_num bw);
t_butter *dsp_alg_br_init(size_t sample_rate, t_num f, t_num bw);

/* update coefficients */

/* f */

void dsp_alg_lp_reset_f(t_butter *st, t_num f);
void dsp_alg_hp_reset_f(t_butter *st, t_num f);
void dsp_alg_bp_reset_f(t_butter *st, t_num f);
void dsp_alg_br_reset_f(t_butter *st, t_num f);

/* bw */

void dsp_alg_bp_reset_bw(t_butter *st, t_num bw);
void dsp_alg_br_reset_bw(t_butter *st, t_num bw);

/* f && bw */

void dsp_alg_bp_reset_f_bw(t_butter *st, t_num f, t_num bw);
void dsp_alg_br_reset_f_bw(t_butter *st, t_num f, t_num bw);

#endif
