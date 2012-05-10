#include <stdlib.h>
#include <math.h>

#include <dsp-alg-types.h>
#include <dsp-alg-num.h>

#include <time.h>


/* phasor */

typedef struct {
    t_num cur;
    t_num recip_sr;
} t_phasor;

t_phasor *dsp_alg_phasor_init(size_t sample_rate, t_num cur)
{
    if (cur >= 1)
        cur = cur - (t_num ) ((size_t) cur);
    if (cur < 0)
        cur = (t_num) 1 - (cur - (t_num) ((size_t) cur));

    t_phasor *res = (t_phasor*) malloc(sizeof(t_phasor));   /* M */

    res->cur = cur;
    res->recip_sr = (t_num) 1.0 / (t_num) sample_rate;
    return res;
}

void dsp_alg_phasor_free(t_phasor *st)
{
    free(st);                                               /* F */
}


void dsp_alg_phasor(size_t n, t_phasor *st, const t_num *in, t_num *out)
{
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

/* linear table */
typedef struct {
    size_t leng;
    t_num *samples;
} t_tabr;

t_tabr *dsp_alg_tabr_init(size_t leng)
{
    t_tabr *res = (t_tabr *) malloc(sizeof(t_tabr));        /* M */
    res->leng    = leng;
    res->samples = (t_num *) calloc(leng + 1, sizeof(t_num));
    return res;
}


void dsp_alg_tabr_free(t_tabr *st)
{
    free(st->samples);                                      /* F */
    free(st);                                               /* F */
}


size_t dsp_alg_tabr_length(t_tabr *a)
{
    return a->leng;
}

t_num *dsp_alg_tabr_samples(t_tabr *a)
{
    return a->samples;
}

void dsp_alg_tabr(size_t n, t_tabr *st, const t_num *in, t_num *out)
{
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


typedef struct {
    t_phasor *phasor;
    t_tabr   *tabr;  
} t_oscil;


t_oscil *dsp_alg_oscil_init(
    size_t sample_rate, t_num phase, t_tabr *tab)
{
    t_oscil *res = (t_oscil *) malloc(sizeof(t_oscil));     /* M */

    res->phasor = dsp_alg_phasor_init(sample_rate, phase);  /* hidden M */
    res->tabr   = tab;

    return res;
}

void dsp_alg_oscil_free(t_oscil *st)
{
    dsp_alg_phasor_free(st->phasor);                        /* hidden F */
    free(st);                                               /* F */
}

void dsp_alg_oscil(size_t n, t_oscil *st, const t_num *in, t_num *out)
{
    dsp_alg_phasor(n, st->phasor, in, out);
    dsp_alg_scale(n, (t_num) ((st->tabr)->leng-1), out, out);
    dsp_alg_tabr(n, st->tabr, out, out);
}

