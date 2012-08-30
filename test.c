#include <stdbool.h>
#include <stdio.h>
#include <dsp-alg-types.h>
#include <dsp-alg-num.h>
#include <dsp-alg-phasor.h>
#include <dsp-alg-filter.h>
#include <dsp-alg-envelope.h>

#define BS 64
#define SR 44100

int main(void)
{
    t_options opt_val;
    t_options *opt = &opt_val;
    options_init( opt, SR, BS );

    t_num 
        *x = sig_init(opt), 
        *y = sig_init(opt);

    t_phasor phs;
    dsp_alg_phasor_init( &phs, opt, 0 );

    dsp_alg_bias(BS, 441, x, x);
    dsp_alg_phasor( &phs, x, y );

    sig_print( opt, y );

    sig_free(x);
    sig_free(y);
    return 0;
}

