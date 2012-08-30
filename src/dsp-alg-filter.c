#include <math.h>
#include <dsp-alg-types.h>
#include <dsp-alg-filter.h>

void dsp_alg_biquad_init(
    t_biquad *out, const t_options *opt,
    t_num a0, t_num a1, t_num a2,
    t_num b1, t_num b2)
{
    out->opt = opt;
    out->a0 = a0;    out->a1 = a1;    out->a2 = a2;
    out->b1 = b1;    out->b2 = b2;
    out->x1 = 0;     out->x2 = 0;     out->y1 = 0;    out->y2 = 0;
}

void dsp_alg_biquad_set(t_biquad *st,
    t_num a0, t_num a1, t_num a2,
    t_num b1, t_num b2)
{
    st->a0 = a0;    st->a1 = a1;    st->a2 = a2;
    st->b1 = b1;    st->b2 = b2;
}

/* biquad filter */

void dsp_alg_biquad(t_biquad *st, const t_num *in, t_num *out)
{
    size_t n = st->opt->bs;
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

static void lp_coeff(t_biquad *st, const t_options *opt, t_num f)
{
    const t_num pi_sr = opt->pi_sr;
    t_num p  = (t_num)1.0 / (t_num) tan(f * pi_sr);
    t_num a0 = (t_num)1.0 /(1 + SQRT2 * p + p * p);
    
    dsp_alg_biquad_init(st, opt, 
        a0, a0+a0, a0,
        2.0*a0*(1.0 - p*p), a0 * (1.0 - SQRT2*p + p*p));        
}

static void hp_coeff(t_biquad *st, const t_options *opt, t_num f)
{
    const t_num pi_sr = opt->pi_sr;
    t_num p   = (t_num) tan(f * pi_sr);
    t_num a0  = (t_num)1.0 /(1.0 + SQRT2*p  + p*p);
    
    dsp_alg_biquad_init(st, opt,
        a0, -(a0+a0), a0, 
        2.0*a0*(p*p - 1.0), a0*(1.0 - SQRT2*p + p*p));
}


static void bp_coeff(t_biquad *st, const t_options *opt, t_num f, t_num bw)
{
    const t_num pi_sr  = opt->pi_sr;
    const t_num tau_sr = opt->tau_sr;
    
    t_num p   = (t_num)1.0 / (t_num) tan(bw * pi_sr);
    t_num n   = 2.0 * cos(f * tau_sr);
    t_num a0  = (t_num)1.0 / (1.0 + p);
    
    dsp_alg_biquad_init(st, opt,
        a0, 0, -a0, 
        -a0*p*n, a0*(p - 1.0));
}

static void br_coeff(t_biquad *st, const t_options *opt, t_num f, t_num bw)
{
    const t_num pi_sr  = opt->pi_sr;
    const t_num tau_sr = opt->tau_sr;
    
    t_num p   = (t_num) tan(bw * pi_sr);
    t_num n   = 2.0 * cos(f * tau_sr);
    t_num a0  = (t_num)1.0 / (1.0 + p);
    
    dsp_alg_biquad_init(st, opt,
        a0, -a0*n, a0, 
        -a0*n, a0*(1.0 - p));
}



void dsp_alg_butter(t_butter *st, const t_num *in, t_num *out)
{
    dsp_alg_biquad(&(st->biquad), in, out);
}

void dsp_alg_lp_init(t_butter *res, const t_options *opt, t_num f)
{
    lp_coeff(&(res->biquad), opt, f);
    res->f = f;
}

void dsp_alg_hp_init(t_butter *res, const t_options *opt, t_num f)
{
    hp_coeff(&(res->biquad), opt, f);
    res->f = f;
}

void dsp_alg_bp_init(t_butter *res, const t_options *opt, t_num f, t_num bw)
{
    bp_coeff(&(res->biquad), opt, f, bw);
    res->f = f;
    res->bw = bw;
}

void dsp_alg_br_init(t_butter *res, const t_options *opt, t_num f, t_num bw)
{
    br_coeff(&(res->biquad), opt, f, bw);
    res->f = f;
    res->bw = bw;
}


/* update coefficients */

/* f */

void dsp_alg_lp_set_f(t_butter *st, t_num f)
{
    if (f != st->f) {
        lp_coeff(&(st->biquad), st->biquad.opt, f);
        st->f = f;
    }
}

void dsp_alg_hp_set_f(t_butter *st, t_num f)
{
    if (f != st->f) {
        hp_coeff(&(st->biquad), st->biquad.opt, f);
        st->f = f;
    }
}

void dsp_alg_bp_set_f(t_butter *st, t_num f)
{
    if (f != st->f) {
        bp_coeff(&(st->biquad), st->biquad.opt, f, st->bw);
        st->f = f;
    }
}

void dsp_alg_br_set_f(t_butter *st, t_num f)
{
    if (f != st->f) {
        br_coeff(&(st->biquad), st->biquad.opt, f, st->bw);
        st->f = f;
    }
}

/* bw */

void dsp_alg_bp_set_bw(t_butter *st, t_num bw)
{
    if (bw != st->bw) {
        bp_coeff(&(st->biquad), st->biquad.opt, st->f, bw);
        st->bw = bw;
    }
}

void dsp_alg_br_set_bw(t_butter *st, t_num bw)
{
    if (bw != st->bw) {
        br_coeff(&(st->biquad), st->biquad.opt, st->f, bw);
        st->bw = bw;
    }
}

/* f && bw */

void dsp_alg_bp_set_f_bw(t_butter *st, t_num f, t_num bw)
{
    if (f == st->f && bw == st->bw)
        return;

    if (f == st->f)
        dsp_alg_bp_set_bw(st, bw);
    else if (bw == st->bw)
        dsp_alg_bp_set_f(st, f);
    else {
        bp_coeff(&(st->biquad), st->biquad.opt, f, bw);
        st->f = f;
        st->bw = bw;
    }
}

void dsp_alg_br_set_f_bw(t_butter *st, t_num f, t_num bw)
{
    if (f == st->f && bw == st->bw)
        return;

    if (f == st->f)
        dsp_alg_br_set_bw(st, bw);
    else if (bw == st->bw)
        dsp_alg_br_set_f(st, f);
    else {
        br_coeff(&(st->biquad), st->biquad.opt, f, bw);
        st->f = f;
        st->bw = bw;
    }
}

