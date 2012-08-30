#include <stdbool.h>
#include <dsp-alg-types.h>

/* resampling */

void dsp_alg_resample(size_t n, t_num ratio, t_num *in, t_num *out)
{
    t_num y0, y1, dx, x;
    
    x = 0;
    size_t xn;
    while (n--) {
        xn = (size_t) x;
        dx = x - (t_num) xn;

        in += xn;
        y0 = *in;
        y1 = *(in+1);

        *out++ = y0 + (y1 - y0) * dx;
        x = dx + ratio;
    }
}

/* ------------------------------------------------------------------ */
/* arithmetic */

/* bias */

void dsp_alg_bias(size_t n, 
        t_num k, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = *in++ + k;
}

/* scale */

void dsp_alg_scale(size_t n, 
        t_num k, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = *in++ * k;
}


/* adder */

void dsp_alg_add(size_t n, 
        const t_num *ina, const t_num *inb, t_num *out)
{
    while (n--)
        *out++ = *ina++ + *inb++;
}

/* sub */

void dsp_alg_sub(size_t n, 
        const t_num *ina, const t_num *inb, t_num *out)
{
    while (n--)
        *out++ = *ina++ - *inb++;
}

/* negation */

void dsp_alg_negate(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = -(*in++);
}

/* multiplier */

void dsp_alg_mul(size_t n, 
        const t_num *ina, const t_num *inb, t_num *out)
{
    while (n--)
        *out++ = *ina++ * *inb++;
}

/* divisor */

void dsp_alg_div(size_t n, 
        const t_num *ina, const t_num *inb, t_num *out)
{
    while (n--)
        *out++ = *ina++ / *inb++;
}

/* reciprocal */

void dsp_alg_recip(size_t n, 
        const t_num *in, t_num *out)
{
    t_num one = 1.0;
    while (n--)
        *out++ = one / (*in++);
}


/* ------------------------------------------------------------- */
/* logic */

void dsp_alg_true(size_t n, 
        t_bool *out)
{
    while (n--)    
        *out++ = (t_bool) true;
}

void dsp_alg_false(size_t n, 
        t_bool *out)
{
    while (n--)
        *out++ = (t_bool) false;
}

void dsp_alg_not(size_t n, 
        const t_bool *in, t_bool *out)
{
    while (n--)
        *out++ = !(*in++);
}

void dsp_alg_and(size_t n, 
        const t_bool *ina, const t_bool *inb, t_bool *out)
{
    while (n--)
        *out++ = *ina++ && *inb++;
}

void dsp_alg_or(size_t n, 
        const t_bool *ina, const t_bool *inb, t_bool *out)
{
    while (n--)
        *out++ = *ina++ || *inb++;
}

void dsp_alg_xor(size_t n, 
        const t_bool *ina, const t_bool *inb, t_bool *out)
{
    while (n--)
        *out++ = *ina++ != *inb++;
}



/* ------------------------------------------------------------- */
/* comparison */

void dsp_alg_eq(size_t n, 
        const t_num *ina, const t_num *inb, t_bool *out)
{
    while (n--)
        *out++ = (t_bool) (*ina++ == *inb++);
}

void dsp_alg_neq(size_t n, 
        const t_num *ina, const t_num *inb, t_bool *out)
{
    while (n--)
        *out++ = (t_bool) (*ina++ != *inb++);
}

void dsp_alg_lt(size_t n, 
        const t_num *ina, const t_num *inb, t_bool *out)
{
    while (n--)
        *out++ = (t_bool) (*ina++ < *inb++);
}

void dsp_alg_gt(size_t n, 
        const t_num *ina, const t_num *inb, t_bool *out)
{
    while (n--)
        *out++ = (t_bool) (*ina++ > *inb++);
}

void dsp_alg_lteq(size_t n, 
        const t_num *ina, const t_num *inb, t_bool *out)
{
    while (n--)
        *out++ = (t_bool) (*ina++ <= *inb++);
}

void dsp_alg_gteq(size_t n, 
        const t_num *ina, const t_num *inb, t_bool *out)
{
    while (n--)
        *out++ = (t_bool) (*ina++ >= *inb++);
}

void dsp_alg_inv(size_t n,
        const t_num *in, t_num *out)
{
    t_num one = 1.0;
    while (n--)
        *out++ = one - (*in++);
}

void dsp_alg_min(size_t n, 
        const t_num *ina, const t_num *inb, t_num *out)
{
    t_num a, b;
    while (n--) {
        a = *ina++;
        b = *inb++;                
        *out++ = (a < b) ? a : b;
    }
}

void dsp_alg_max(size_t n, 
        const t_num *ina, const t_num *inb, t_num *out)
{
    t_num a, b;
    while (n--) {
        a = *ina++;
        b = *inb++;
        *out++ = (a < b) ? b : a;
    }
}

/* --------------------------------------------------- */
/* wrappers */

void dsp_alg_wrap(size_t n,
        const t_num *in, t_num *out)
{
    t_num a;
    while (n--) {
        a = *in++;
        if (a >= 1.0)
            a = a - (int) a;
        else if (a < 0.0)
            a = 1.0 + a - (int) a;

        *out++ = a;
    }
}

void dsp_alg_clip(size_t n,
        const t_num *in, t_num *out)
{
    t_num a;
    while (n--) {
        a = *in++;
        if (a > 1.0)
            a = 1.0;
        else if (a < 0.0)
            a = 0.0;

        *out++ = a;
    }
}



