#include <math.h>

#include <dsp-alg-types.h>
#include <dsp-alg-phasor.h>


void dsp_alg_scale_tab(t_tabr *tab, t_num val) 
{
	size_t n = dsp_alg_tabr_length(tab);	
	t_num *x = dsp_alg_tabr_samples(tab);
	
	while (n--) 
		*x++ *= val;
}

void dsp_alg_norm_tab(t_tabr *tab)
{
	size_t n = dsp_alg_tabr_length(tab);	
	t_num *x = dsp_alg_tabr_samples(tab);

	t_num max_val = 0;	
	while (n--)	{
		t_num val = fabs(*x++);
		max_val = (val < max_val) ? max_val : val;
	} 		

	if (max_val > EPS) 
		dsp_alg_scale_tab(tab, 1.0/max_val);	
}


/* -----------------------------------------------------
	sum of pure harmonics

	a[i] = amp[i] * cos( freq[i] * x + phase[i] )
	--------------------------------------------------- */  
t_tabr* dsp_alg_cos_tab(
	int is_normalised,		    /* normalisation flag */
	size_t leng, 				/* table length */
	size_t harmonics_count, 	/* number of harmonics */
	const t_num *amps, 			/* amplitudes */
    const t_num *freqs,         /* frequencies */
	const t_num *phases)		/* phases */
{
	t_tabr *res = dsp_alg_tabr_init(leng);

	t_num *x = dsp_alg_tabr_samples(res);
	t_num d = TAU / (t_num) leng;
	
    int k;
	t_num phi = 0;
    const t_num *amp, *freq, *phase;
    /* add guard point */
    leng++;
	while (leng--) {
		t_num val = 0;
	    k = harmonics_count - 1;
        amp   = amps + k;
        freq  = freqs + k;
        phase = phases + k;

        /* summs from last to first 
         * ie harmonic series
         */
        int n = harmonics_count;
        while (n--)
			val += (*amp--) * cos((*freq--)*phi + TAU* (*phase--));
			
		*x++ = val;
        phi+=d;
    }

	if (is_normalised)	dsp_alg_norm_tab(res);

	return res;
}


/* ----------------------------------------------
	Linear table	
  -------------------------------------------- */

t_tabr* dsp_alg_lin_tab(
    int is_normalised,
	size_t leng,
	size_t points_count,
	const t_num *points)
{
    t_tabr *res = dsp_alg_tabr_init(leng);
    
    t_num *y = dsp_alg_tabr_samples(res);
    
    /* calculate total length of envelope */
    t_num total_leng = 0;
    int i;
    for (i=1; i<points_count; i+=2)
        total_leng += points[i];

    const t_num *p = points;    

    /* table length and one guard point  */
    size_t safety_leng = leng + 1;
    int dur;
    t_num base;
    t_num curr = 0;
    t_num x_incr = 1.0/((t_num) leng-1);
    t_num y_incr;
    for (i=0; i<points_count-1; i+=2, p+=2) {
        t_num pa = p[0];
        t_num dt = p[1]/total_leng;
        t_num pb = p[2];

        /* if point is beyond current interval
           let's skip to next one */
        if (curr > dt) {
            curr -= dt;         
        }
        else {
            dur = (int) ((t_num) leng * (dt - curr));

            if (dur) {
                base = pa + (pb - pa) * curr / dt;
                y_incr = (pb - base)/(t_num) dur;
            
                while (curr <= dt) {
                    *y++ = base;    safety_leng--;
                    base += y_incr;
                    curr += x_incr;
                }
            }
            
            curr -= dt;
        }
    }
	
    /* if there are some values left fill them
       with last  value in the table */
    /* i can not guarantee that all values will 
       be filled in the cycle oh'my */
    if (safety_leng > 0) {
        t_num last_val = points[(int) points_count-1];
        while (safety_leng--) 
            *y++ = last_val;
    }

    if (is_normalised)	dsp_alg_norm_tab(res);
    
    return res;
}

// todo

/* ----------------------------------------------
	Cubic splines table	
  -------------------------------------------- */

/* ----------------------------------------------
	Exponential table	
  -------------------------------------------- */

/* ----------------------------------------------
	Wav table	
  -------------------------------------------- */

/* ----------------------------------------------
	Windows	
  -------------------------------------------- */


