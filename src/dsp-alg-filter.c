#include <stdlib.h>
#include <math.h>

#include <dsp-alg-types.h>


typedef struct {
    t_num a0, a1, a2, b1, b2;   /* coefficients */
    t_num x1, x2, y1, y2;       /* delayed samples */
} t_biquad;


t_biquad *dsp_alg_biquad_init(
    t_num a0, t_num a1, t_num a2,
    t_num b1, t_num b2)
{
    t_biquad *out = (t_biquad *) malloc(sizeof(t_biquad));      /* M */

    out->a0 = a0;    out->a1 = a1;    out->a2 = a2;
    out->b1 = b1;    out->b2 = b2;
    out->x1 = 0;     out->x2 = 0;     out->y1 = 0;    out->y2 = 0;

    return out;
}

void dsp_alg_biquad_reset(t_biquad *st,
    t_num a0, t_num a1, t_num a2,
    t_num b1, t_num b2)
{
    st->a0 = a0;    st->a1 = a1;    st->a2 = a2;
    st->b1 = b1;    st->b2 = b2;
}

void dsp_alg_biquad_free(t_biquad *in) 
{
    free(in);                                                   /* F */
}

/* biquad filter */

void dsp_alg_biquad(size_t n, t_biquad *st, const t_num *in, t_num *out)
{
    t_num x1, x2, y1, y2; 
    x1 = st->x1; x2 = st->x2; y1 = st->y1; y2 = st->y2;

    while (n--) {
        *out = st->a0 * *in + st->a1 * x1 
                            + st->a2 * x2
             - st->b1 * y1  - st->b2 * y2;

        x2 = x1;        x1 = *in++;
        y2 = y1;        y1 = *out++;
    }

    st->x1 = x1;    st->x2 = x2;    st->y1 = y1;    st->y2 = y2;
}


/* Butterworth filters */
/* f  -- central frequency
   bw -- bandwidth
*/

void lp_coeff(t_biquad *st, t_num pi_sr, t_num f)
{
    t_num p  = (t_num)1.0 / (t_num) tan(f * pi_sr);
    t_num a0 = (t_num)1.0 /(1 + SQRT2 * p + p * p);
    
    return dsp_alg_biquad_reset(st, a0, a0+a0, a0,
            2.0*a0*(1.0 - p*p), a0 * (1.0 - SQRT2*p + p*p));        
}

void hp_coeff(t_biquad *st, t_num pi_sr, t_num f)
{
    t_num p   = (t_num) tan(f * pi_sr);
    t_num a0  = (t_num)1.0 /(1.0 + SQRT2*p  + p*p);
    
    return dsp_alg_biquad_reset(st, a0, -(a0+a0), a0, 
            2.0*a0*(p*p - 1.0), a0*(1.0 - SQRT2*p + p*p));
}


void bp_coeff(t_biquad *st, t_num pi_sr, t_num tau_sr, t_num f, t_num bw)
{
    t_num p   = (t_num)1.0 / (t_num) tan(bw * pi_sr);
    t_num n   = 2.0 * cos(f * tau_sr);
    t_num a0  = (t_num)1.0 / (1.0 + p);
    
    return dsp_alg_biquad_reset(st, a0, 0, -a0, 
            -a0*p*n, a0*(p - 1.0));
}

void br_coeff(t_biquad *st, t_num pi_sr, t_num tau_sr, t_num f, t_num bw)
{
    t_num p   = (t_num) tan(bw * pi_sr);
    t_num n   = 2.0 * cos(f * tau_sr);
    t_num a0  = (t_num)1.0 / (1.0 + p);
    
    return dsp_alg_biquad_reset(st, a0, -a0*n, a0, 
            -a0*n, a0*(1.0 - p));
}


typedef struct {
    t_biquad *biquad;
    t_num pi_sr;
    t_num tau_sr;
    t_num f;
    t_num bw;
} t_butter; 

static t_butter *dsp_alg_butter_init(size_t sample_rate)
{
    t_butter *res = (t_butter *) malloc(sizeof(t_butter));  /* M */

    res->biquad = dsp_alg_biquad_init(0, 0, 0, 0, 0);       /* hidden M */
    res->pi_sr = pi_sr(sample_rate);
    res->tau_sr = tau_sr(sample_rate);
    res->f = 0;
    res->bw = 0;

    return res;
}

void dsp_alg_butter_free(t_butter *st)
{
    dsp_alg_biquad_free(st->biquad);                        /* F */
    free(st);                                               /* F */
}

void dsp_alg_butter(size_t n, t_butter *st, const t_num *in, t_num *out)
{
    dsp_alg_biquad(n, st->biquad, in, out);
}

t_butter *dsp_alg_lp_init(size_t sample_rate, t_num f)
{
    t_butter *res = dsp_alg_butter_init(sample_rate);
    lp_coeff(res->biquad, res->pi_sr, f);
    res->f = f;

    return res;
}

t_butter *dsp_alg_hp_init(size_t sample_rate, t_num f)
{
    t_butter *res = dsp_alg_butter_init(sample_rate);
    hp_coeff(res->biquad, res->pi_sr, f);
    res->f = f;

    return res;
}

t_butter *dsp_alg_bp_init(size_t sample_rate, t_num f, t_num bw)
{
    t_butter *res = dsp_alg_butter_init(sample_rate);
    bp_coeff(res->biquad, res->pi_sr, res->tau_sr, f, bw);
    res->f = f;
    res->bw = bw;

    return res;
}

t_butter *dsp_alg_br_init(size_t sample_rate, t_num f, t_num bw)
{
    t_butter *res = dsp_alg_butter_init(sample_rate);
    br_coeff(res->biquad, res->pi_sr, res->tau_sr, f, bw);
    res->f = f;
    res->bw = bw;

    return res;
}


/* update coefficients */

/* f */

void dsp_alg_lp_reset_f(t_butter *st, t_num f)
{
    if (f != st->f) {
        lp_coeff(st->biquad, st->pi_sr, f);
        st->f = f;
    }
}

void dsp_alg_hp_reset_f(t_butter *st, t_num f)
{
    if (f != st->f) {
        hp_coeff(st->biquad, st->pi_sr, f);
        st->f = f;
    }
}

void dsp_alg_bp_reset_f(t_butter *st, t_num f)
{
    if (f != st->f) {
        bp_coeff(st->biquad, st->pi_sr, st->tau_sr, f, st->bw);
        st->f = f;
    }
}

void dsp_alg_br_reset_f(t_butter *st, t_num f)
{
    if (f != st->f) {
        br_coeff(st->biquad, st->pi_sr, st->tau_sr, f, st->bw);
        st->f = f;
    }
}

/* bw */

void dsp_alg_bp_reset_bw(t_butter *st, t_num bw)
{
    if (bw != st->bw) {
        bp_coeff(st->biquad, st->pi_sr, st->tau_sr, st->f, bw);
        st->bw = bw;
    }
}

void dsp_alg_br_reset_bw(t_butter *st, t_num bw)
{
    if (bw != st->bw) {
        br_coeff(st->biquad, st->pi_sr, st->tau_sr, st->f, bw);
        st->bw = bw;
    }
}

/* f && bw */

void dsp_alg_bp_reset_f_bw(t_butter *st, t_num f, t_num bw)
{
    if (f == st->f && bw == st->bw)
        return;

    if (f == st->f)
        dsp_alg_bp_reset_bw(st, bw);
    else if (bw == st->bw)
        dsp_alg_bp_reset_f(st, f);
    else {
        bp_coeff(st->biquad, st->pi_sr, st->tau_sr, f, bw);
        st->f = f;
        st->bw = bw;
    }
}

void dsp_alg_br_reset_f_bw(t_butter *st, t_num f, t_num bw)
{
    if (f == st->f && bw == st->bw)
        return;

    if (f == st->f)
        dsp_alg_br_reset_bw(st, bw);
    else if (bw == st->bw)
        dsp_alg_br_reset_f(st, f);
    else {
        br_coeff(st->biquad, st->pi_sr, st->tau_sr, f, bw);
        st->f = f;
        st->bw = bw;
    }
}

