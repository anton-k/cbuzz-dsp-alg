#ifndef DSP_ALG_BUFFER_H 
#define DSP_ALG_BUFFER_H 

#include "dsp-alg-types.h"

/* circular buffer */
/* size is whole multiplier of cr */
typedef struct _buf t_buf;

/* init buffer
    dt -- length of the bufer in samples
 */
t_buf *dsp_alg_buf_init_with_time(
    size_t sample_rate, size_t block_size, t_num dt); 
t_buf *dsp_alg_buf_init(size_t block_size, size_t num_of_frames); 

void dsp_alg_buf_free(t_buf *in);
size_t dsp_alg_buf_length(t_buf *a);

void dsp_alg_buf_trigger(t_buf *a, 
    void (*fn)(void *, size_t, t_num *), void *);

void dsp_alg_buf_trigger_part(t_buf *a, 
    void (*fn)(void *, size_t, t_num *), void *st);

void dsp_alg_buf_tirgger2(t_buf *a, t_buf *b, 
    void (*fn)(void *, size_t, t_num *, t_num *), void *st);

void dsp_alg_buf_trigger2_part(t_buf *a, t_buf *b, 
    void (*fn)(void *, size_t, t_num *, t_num *), void *st);

void dsp_alg_bufw(size_t n, t_buf *st, const t_num *in);
void dsp_alg_bufr(size_t n, t_buf *st, t_num *out);

typedef struct _delay t_delay;

t_delay *dsp_alg_delay_init(
    size_t control_rate, size_t block_size,
    const t_buf *buf, t_num dt);

void dsp_alg_delay_free(t_delay *st);
void dsp_alg_delay(size_t n, const t_delay *st, t_num *out);
typedef struct _vdelay t_vdelay;

t_vdelay *dsp_alg_vdelay_init(t_buf *buf);
void dsp_alg_vdelay_free(t_vdelay *st);
void dsp_alg_vdelay(size_t n, t_vdelay *st, const t_num *in, t_num *out);


/* ----------------------------------------------------------- */
/* stereo buffer */

/* circular buffer */
/* size is whole multiplier of cr */
typedef struct _buf2 t_buf2;

/* init buffer
    dt -- length of the bufer in samples

    each frame contains 2*block_size samples
 */
t_buf2 *dsp_alg_buf2_init(size_t block_size, size_t num_of_frames);
t_buf2 *dsp_alg_buf2_init_with_time(
    size_t control_rate, size_t block_size, t_num dt);
void dsp_alg_buf2_free(t_buf2 *in);
size_t dsp_alg_buf2_length(t_buf2 *a);
void dsp_alg_buf2w(size_t n, t_buf2 *st, const t_num *ina, const t_num *inb);
void dsp_alg_buf2r(size_t n, t_buf2 *st, t_num *outa, t_num *outb);

/* function takes number of !frames! not samples */
void dsp_alg_buf2_trigger(t_buf2 *a, 
    void (*fn)(void *, size_t, t_num *), void *);

void dsp_alg_buf2_trigger_part(t_buf2 *a, 
    void (*fn)(void *, size_t, t_num *), void *st);

#endif


