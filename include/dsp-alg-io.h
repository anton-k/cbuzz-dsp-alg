#ifndef DSP_ALG_IO_H
#define DSP_ALG_IO_H

#include <stdlib.h>

#include "dsp-alg-types.h"
#include "dsp-alg-buffer.h"

/* maybe whole idea of reading with specific buffer reader position
 * is flawed. consider better solution, just
 * read/write to buffers and let the patcher layer
 * decide when to read/write */

typedef struct _sig_printf t_sig_printf;

t_sig_printf *dsp_alg_sig_printf_init(size_t block_size, const char *label);
void dsp_alg_sig_printf_free(t_sig_printf *st);
void dsp_alg_sig_printf(size_t n, t_sig_printf *st, const t_num *in); 

void dsp_alg_set_sig_printf_buf_length(size_t val);

typedef struct _sig_fprintf t_sig_fprintf;

t_sig_fprintf *dsp_alg_sig_fprintf_init(
    size_t block_size, const char *filename, const char *divisor);

void dsp_alg_sig_fprintf_free(t_sig_fprintf *st);
void dsp_alg_sig_fprintf(size_t n, t_sig_fprintf *st, const t_num *in);

void dsp_alg_set_sig_fprintf_buf_length(size_t val);

typedef struct _wavw t_wavw;

t_wavw *dsp_alg_wavw_init(size_t sample_rate, size_t block_size, 
    const char *filename);
void dsp_alg_wavw_free(t_wavw *st);
void dsp_alg_wavw(size_t n, t_wavw *st, const t_num *ina, const t_num *inb);

void dsp_alg_set_wavw_buf_length(size_t val);

typedef struct _wavr t_wavr;

t_wavr *dsp_alg_wavr_init(size_t sample_rate, 
    const char *filename, t_buf2 *buf);
void dsp_alg_wavr_free(t_wavr *st);
void dsp_alg_wavr(t_wavr *st);

#endif

