#include <stdbool.h>
#include <stdio.h>
#include <dsp-alg.h>

#define N 64
#define SR 44100



void test_phasor(void);
void test_linseg_reset(void);
void test_linseg_append(void);
void test_expseg(void);
void test_tabr(void);
void test_sndfilew(void);

int main(void)
{
    test_sndfilew();

    return 0;
}

void init_sigs(t_num **x, t_num **y, t_num **z)
{
    *x = sig_init(N);    *y = sig_init(N);    *z = sig_init(N);
}

void free_sigs(t_num *x, t_num *y, t_num *z)
{
    sig_free(x);    sig_free(y);    sig_free(z);
}


void test_tabr(void)
{
    t_num *x, *y, *z;
    init_sigs(&x, &y, &z);
    t_sig_fprintf *st_fprintf = 
        dsp_alg_sig_fprintf_init(N, "fig", "\n");


    size_t harmonics_count = 5;
    t_num freqs[]  = {1, 2, 3, 4, 5, 6};
    t_num amps []  = {1, 0.5, 0.3, 0.25, 0.125, 0.15};
    t_num phases[] = {0, 0, 0, 0, 0, 0};

    t_tabr *tab = dsp_alg_cos_tab(true, 40, harmonics_count, 
        amps, freqs, phases);

    t_phasor *phs = dsp_alg_phasor_init(SR, 0.0);

    t_oscil *st = dsp_alg_oscil_init(SR, 0.0, tab);
    dsp_alg_hold(N, 440, x);

    size_t n = 2;
    while (n--) {
        dsp_alg_oscil(N, st, x, z);
//        dsp_alg_phasor(N, phs, x, z);
        dsp_alg_sig_fprintf(N, st_fprintf, z);            
    }    

    dsp_alg_oscil_free(st);
    dsp_alg_tabr_free(tab);
    dsp_alg_sig_fprintf_free(st_fprintf);
    free_sigs(x, y, z);
}

void test_phasor(void)
{
    t_num *x, *y, *z;
    init_sigs(&x, &y, &z);
    
    t_phasor *st_phs = dsp_alg_phasor_init(SR, 0.0);
    t_sig_fprintf *st_fprintf = 
        dsp_alg_sig_fprintf_init(N, "fig", "\n");

    dsp_alg_hold(N, 3, x);
    dsp_alg_hold(N, 2, y);

    size_t n = 1000;
    while (n--) {
        dsp_alg_phasor(N, st_phs, x, z);
        dsp_alg_sig_fprintf(N, st_fprintf, z);
    }
      
    free_sigs(x, y, z);
    dsp_alg_phasor_free(st_phs);
    dsp_alg_sig_fprintf_free(st_fprintf);
}


void test_linseg_append(void)
{
    t_num *x, *y, *z;
    init_sigs(&x, &y, &z);

    t_linseg *st = dsp_alg_linseg_init(SR, N);
    dsp_alg_linseg_append(st, 10, 1);
    dsp_alg_linseg_append(st, 10, 0.5);
    dsp_alg_linseg_append(st, 40, 0.5);
    dsp_alg_linseg_append(st, 40, 0.01);

    t_sig_fprintf *st_fprintf = 
        dsp_alg_sig_fprintf_init(N, "fig", "\n");

    t_phasor *st_phs = dsp_alg_phasor_init(SR, 0.0);

    dsp_alg_hold(N, 3, x);

    size_t n = 1000;
    while (n--) {
        dsp_alg_linseg(N, st, z);
        dsp_alg_phasor(N, st_phs, x, y);
        dsp_alg_mul(N, z, y, z);
        dsp_alg_sig_fprintf(N, st_fprintf, z);
    }

    free_sigs(x, y, z);
    dsp_alg_sig_fprintf_free(st_fprintf);
    dsp_alg_linseg_free(st);
}


void test_linseg_reset(void)
{
    t_num *x, *y, *z;
    init_sigs(&x, &y, &z);

    t_linseg *st = dsp_alg_linseg_init(SR, N);
    dsp_alg_linseg_append(st, 10, 1);
    dsp_alg_linseg_append(st, 10, 0.5);
    dsp_alg_linseg_append(st, 40, 0.5);
    dsp_alg_linseg_append(st, 40, 0.01);

    t_sig_fprintf *st_fprintf = 
        dsp_alg_sig_fprintf_init(N, "fig", "\n");

    dsp_alg_hold(N, 3, x);

    size_t n = 1000;
    while (n--) {
        if (n == 300) {
            dsp_alg_linseg_reset(st);
            dsp_alg_linseg_append(st, 10, 1);
            dsp_alg_linseg_append(st, 10, 0.8);
            dsp_alg_linseg_append(st, 0, 0.6);
            dsp_alg_linseg_append(st, 5, 0.2); 
        }

        dsp_alg_linseg(N, st, z);
        dsp_alg_sig_fprintf(N, st_fprintf, z);
    }

    free_sigs(x, y, z);
    dsp_alg_sig_fprintf_free(st_fprintf);
    dsp_alg_linseg_free(st);
}

void test_expseg(void)
{
    t_num *x, *y, *z;
    init_sigs(&x, &y, &z);

    t_linseg *st = dsp_alg_expseg_init(SR, N);
    dsp_alg_expseg_append(st, 10, 1);
    dsp_alg_expseg_append(st, 60, 0.01);

    t_sig_fprintf *st_fprintf = 
        dsp_alg_sig_fprintf_init(N, "fig", "\n");

    size_t n = 1000;
    while (n--) {
        dsp_alg_expseg(N, st, z);
        dsp_alg_sig_fprintf(N, st_fprintf, z);
    }

    free_sigs(x, y, z);
    dsp_alg_sig_fprintf_free(st_fprintf);
    dsp_alg_expseg_free(st);
}



void test_sndfilew(void)
{
    t_num *x, *y, *z;
    init_sigs(&x, &y, &z);
    t_sig_fprintf *st_fprintf = 
        dsp_alg_sig_fprintf_init(N, "fig", "\n");

    t_sndfilew *snd = dsp_alg_wav16w_init(SR, N, "test.wav", 64);
  
    t_linseg *st_env = dsp_alg_linseg_init(SR, N);
    dsp_alg_linseg_append(st_env, 0.5, 1);
    dsp_alg_linseg_append(st_env, 0.5, 0.5);
    dsp_alg_linseg_append(st_env, 2, 0.5);
    dsp_alg_linseg_append(st_env, 1, 0.01);

    size_t harmonics_count = 1;
    t_num freqs[]  = {1, 2, 3, 4, 5, 6};
    t_num amps []  = {1, 0.5, 0.3, 0.25, 0.20, 0.15};
    t_num phases[] = {0, 0, 0, 0, 0, 0};

    t_tabr *tab = dsp_alg_cos_tab(true, 100, harmonics_count, 
        amps, freqs, phases);

    t_oscil *st = dsp_alg_oscil_init(SR, 0.0, tab);
    dsp_alg_hold(N, 440, x);

    size_t n = 10000;
    while (n--) {
        dsp_alg_oscil(N, st, x, z);
        dsp_alg_linseg(N, st_env, y);
        dsp_alg_mul(N, z, y, z);
        dsp_alg_scale(N, 0.5, z, z);
        
        dsp_alg_sndfilew(N, snd, z, z);
        dsp_alg_sig_fprintf(N, st_fprintf, z);
    }
 
    free_sigs(x, y, z);
    dsp_alg_linseg_free(st_env);
    dsp_alg_sndfilew_free(snd);
    dsp_alg_oscil_free(st);
    dsp_alg_sig_fprintf_free(st_fprintf);
}

