#include <math.h>
#include <dsp-alg-types.h>


void dsp_alg_sin(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) sin((double) *in++);
}


void dsp_alg_cos(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) cos((double) *in++);
}

void dsp_alg_tan(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) tan((double) *in++);
}


void dsp_alg_asin(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) asin((double) *in++);
}


void dsp_alg_acos(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) acos((double) *in++);
}


void dsp_alg_atan(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) atan((double) *in++);
}

void dsp_alg_atan2(size_t n, 
        const t_num *ina, const t_num *inb, t_num *out)
{
    while (n--)
        *out++ = (t_num) atan2((double) *ina++, (double) *inb++);
}

void dsp_alg_sinh(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) sinh((double) *in++);
}


void dsp_alg_cosh(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) cosh((double) *in++);
}

void dsp_alg_tanh(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) tanh((double) *in++);
}


void dsp_alg_exp(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) exp((double) *in++);
}


void dsp_alg_log(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) log((double) *in++);
}

void dsp_alg_log10(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) log10((double) *in++);
}

void dsp_alg_pow(size_t n, 
        const t_num *ina, const t_num *inb, t_num *out)
{
    while (n--)
        *out++ = (t_num) pow((double) *ina++, (double) *inb++);
}


void dsp_alg_sqrt(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) sqrt((double) *in++);
}


void dsp_alg_ceil(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) ceil((double) *in++);
}

void dsp_alg_floor(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) floor((double) *in++);
}


void dsp_alg_fabs(size_t n, 
        const t_num *in, t_num *out)
{
    while (n--)
        *out++ = (t_num) fabs((double) *in++);
}

void dsp_alg_fmod(size_t n, 
        const t_num *ina, const t_num *inb, t_num *out)
{
    while (n--)
        *out++ = (t_num) fmod((double) *ina++, (double) *inb++);
}

