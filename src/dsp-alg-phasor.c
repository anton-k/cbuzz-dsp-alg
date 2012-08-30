#include <math.h>
#include <dsp-alg-types.h>
#include <dsp-alg-num.h>

#include <dsp-alg-phasor.h>
#include <time.h>

/* phasor */

void dsp_alg_phasor_init(t_phasor *res, const t_options *opt, t_num cur)
{
    if (cur >= 1)
        cur = cur - (t_num ) ((size_t) cur);
    if (cur < 0)
        cur = (t_num) 1 - (cur - (t_num) ((size_t) cur));

    res->bs = opt->bs;
    res->cur = cur;
    res->recip_sr = opt->recip_sr ;
}

void dsp_alg_phasor(t_phasor *st, const t_num *in, t_num *out)
{
    size_t n = st->bs;
    t_num k = st->recip_sr;
    t_num cur = st->cur;

    while (n--) {
        *out++ = cur;
        cur += (k * *in++);
        if (cur < 0) 
            cur += 1;
        else if (cur >= 1)
            cur -= 1;
    }

    st->cur = cur;
}


void dsp_alg_tabr_init(t_tabr *res, const t_options *opt, 
    size_t leng, t_num *samples)
{
    res->leng    = leng;
    res->samples = samples;
    res->bs      = opt->bs;
}

size_t dsp_alg_tabr_length(t_tabr *a)
{
    return a->leng;
}

t_num *dsp_alg_tabr_samples(t_tabr *a)
{
    return a->samples;
}

void dsp_alg_tabr(t_tabr *st, const t_num *in, t_num *out)
{
    size_t n = st->bs;
    int x0;
    t_num fx, x, dx, y0, y1;
    size_t leng = st->leng;
    while (n--) {
        x = *in++;        
        fx = (t_num) floor((double) x);
        dx = x - fx;
        x0 = (int) fx;

        /* check bounds */
        while (x0 < 0) 
            x0 += leng;
        while (x0 >= leng)
            x0 -= leng;
        
        y0 = st->samples[x0];
        y1 = st->samples[x0+1];
        *out++ = y0 + (y1 - y0) * dx;
    }
}

void dsp_alg_oscil_init(
    t_oscil *res, const t_options *opt, t_phasor *phasor, t_tabr *tabr)
{
    res->phasor = phasor;
    res->tabr   = tabr;
}

void dsp_alg_oscil(t_oscil *st, const t_num *in, t_num *out)
{
    dsp_alg_phasor(st->phasor, in, out);
    dsp_alg_scale(st->phasor->bs, (t_num) ((st->tabr)->leng-1), out, out);
    dsp_alg_tabr(st->tabr, out, out);
}

