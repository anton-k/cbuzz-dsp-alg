#ifndef DSP_ALG_TABLE_H
#define DSP_ALG_TABLE_H

#include "dsp-alg-types.h"
#include "dsp-alg-phasor.h"

void dsp_alg_scale_tab(t_tabr *tab, t_num val); 
void dsp_alg_norm_tab(t_tabr *tab);

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
	const t_num *phases);   	/* phases */


/* ----------------------------------------------
	Linear table	
  -------------------------------------------- */

t_tabr* dsp_alg_lin_tab(
    int is_normalised,
	size_t leng,
	size_t points_count,
	const t_num *points);

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

#endif
