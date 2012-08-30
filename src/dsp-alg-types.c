
#include <stdlib.h>
#include <stdio.h>

#include <dsp-alg-types.h>

void options_init( t_options *opt, 
    size_t sample_rate, size_t block_size )
{
    opt->sr         = sample_rate;
    opt->bs         = block_size;
    opt->cr         = sample_rate / block_size;
    
    opt->pi_sr      = PI * (t_num) sample_rate;
    opt->tau_sr     = TAU * (t_num) sample_rate;
    opt->recip_sr   = (t_num) 1.0 / (t_num) sample_rate;

    opt->eps        = EPS;
    opt->pi         = PI;
    opt->tau        = TAU;
    opt->sqrt2      = SQRT2;
}

t_num *sig_init(const t_options *opt)
{
    return (t_num *) calloc(opt->bs, sizeof(t_num));
}

void sig_free(t_num *a)
{
    free(a);
}


t_bool *sig_bool_init(const t_options *opt)
{
    return (t_bool *) calloc(opt->bs, sizeof(t_bool));
}

void sig_bool_free(t_bool *a)
{
    free(a);
}

void sig_print(const t_options *opt, const t_num *in)
{
    int i;
    int n = opt->bs;
    for (i = 0; i < n; i++) {
        printf("%.2f", *in++);
        if (i % (n-1) != 0 || i == 0)
            printf(", ");
    }
    printf("\n");
}



