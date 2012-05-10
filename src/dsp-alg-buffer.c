#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <dsp-alg-types.h>


/* circular buffer */
/* size is whole multiplier of cr */
typedef struct {
    t_num *samples;     /* samples */
    t_num *curr;        /* current position */
    size_t dur;         /* length in cr */
    size_t count_to_last; /* number of blocks to end */
    size_t leng;        /* length in sr */
} t_buf;

t_buf *dsp_alg_buf_init(size_t block_size, size_t num_of_frames) 
{
    size_t dur = num_of_frames;
    t_buf *res = (t_buf *) malloc(sizeof(t_buf));               /* M */

    res->dur = dur;
                                                                /* M */
    res->samples = (t_num *) calloc(dur * block_size, sizeof(t_num));
    res->curr = res->samples;
    res->count_to_last = res->dur;
    res->leng = dur * block_size;

    return res;
}


/* init buffer
    dt -- length of the bufer in samples
 */
t_buf *dsp_alg_buf_init_with_time(
    size_t sample_rate, size_t block_size, t_num dt) 
{
    return dsp_alg_buf_init(block_size, 
            (size_t) (dt * (t_num) control_rate(sample_rate, block_size)));
}


void dsp_alg_buf_free(t_buf *in)
{
    free(in->samples);                                          /* F */
    free(in);                                                   /* F */
}

size_t dsp_alg_buf_length(t_buf *a)
{
    return a->leng;
}


void dsp_alg_buf_trigger(t_buf *a, 
    void (*fn)(void *, size_t, t_num *), void *st)
{
    if (a->count_to_last == 0)
        fn(st, a->leng, a->samples);        
}

void dsp_alg_buf_trigger_part(t_buf *a, 
    void (*fn)(void *, size_t, t_num *), void *st)
{
    fn(st, a->curr - a->samples, a->samples);
}

void dsp_alg_buf_tirgger2(t_buf *a, t_buf *b, 
    void (*fn)(void *, size_t, t_num *, t_num *), void *st)
{
    if (a->count_to_last == 0)
        fn(st, a->leng, a->samples, b->samples);        
}

void dsp_alg_buf_trigger2_part(t_buf *a, t_buf *b, 
    void (*fn)(void *, size_t, t_num *, t_num *), void *st)
{
    fn(st, a->curr - a->samples, a->samples, b->samples);
}



static t_buf *buf_shallow_copy(const t_buf *src)
{
    t_buf *res = (t_buf *) malloc(sizeof(t_buf));               /* M */ 
    return (t_buf *) memcpy(res, src, sizeof(t_buf));
}

static void buf_shallow_free(t_buf *st)
{
    free(st);
}

/* write single block to buffer */
void dsp_alg_bufw(size_t n, t_buf *st, const t_num *in)
{
    if (st->count_to_last == 0) {
        st->curr = st->samples;
        st->count_to_last = st->dur;
    }

    t_num *curr = st->curr;
    while (n--) 
        *curr++ = *in++;
    st->curr = curr;

    (st->count_to_last)--;
}

/* read single block from buffer */
void dsp_alg_bufr(size_t n, t_buf *st, t_num *out)
{
    if (st->count_to_last == 0) {
        st->curr = st->samples;
        st->count_to_last = st->dur;
    }

    t_num *curr = st->curr;
    while (n--)
        *out++ = *curr++;

    st->curr = curr;

    (st->count_to_last)--;
}

static void bufr_vdelay(size_t n, t_buf *st, const t_num *in, t_num *out)
{
    t_num y0, y1;
    t_num x, fx, dx;    
    t_num *x0, *x1;

    if (st->count_to_last == 0) {
        st->curr = st->samples;
        st->count_to_last = st->dur;
    }

    t_num *curr = st->curr;
    while (n--) {
       x = *in++;
       fx = (t_num) floor((double) x);
       
       while (fx >= st->leng)
           fx -= st->leng;

       dx = x - fx;
       x0 = curr - (int) fx;

       while (x0 < st->samples) 
           x0 += st->leng;       

       x1 = (x0 - st->samples < st->leng - 1) ? x0+1 : st->samples;
       y0 = *x0;
       y1 = *x1;

       *out++ = y0 + (y1 - y0) * dx;
       curr++;
    }
    st->curr = curr;

    (st->count_to_last)--;
}



typedef struct {
    t_buf *buf;
} t_delay;

t_delay *dsp_alg_delay_init(
    size_t control_rate, size_t block_size,
    const t_buf *buf, t_num dt)
{
    t_delay *res = (t_delay *) malloc(sizeof(t_delay));         /* M */
    t_buf   *b   = buf_shallow_copy(buf);
        
    /* delay offset */
    size_t offset = (size_t) ((t_num) buf->dur - dt * (t_num) control_rate);
    if (offset < 0) offset = 0;
    b->curr             = b->samples + offset * block_size;
    b->count_to_last    = b->dur - offset;

    res->buf = b;
    return res;
}

void dsp_alg_delay_free(t_delay *st) 
{
    buf_shallow_free(st->buf);                                  /* F */
    free(st);                                                   /* F */
}

void dsp_alg_delay(size_t n, const t_delay *st, t_num *out)
{
    dsp_alg_bufr(n, st->buf, out);
}

typedef struct {
    t_buf *buf;
} t_vdelay;

t_vdelay *dsp_alg_vdelay_init(t_buf *buf)
{
    t_vdelay *res = (t_vdelay *) malloc(sizeof(t_vdelay));         /* M */

    res->buf = buf_shallow_copy(buf);
    return res;
}

void dsp_alg_vdelay_free(t_vdelay *st)
{
    buf_shallow_free(st->buf);
    free(st);                                                       /* F */
}

void dsp_alg_vdelay(size_t n, t_vdelay *st, const t_num *in, t_num *out)
{
    bufr_vdelay(n, st->buf, in, out);
}


/* ----------------------------------------------------------- */
/* stereo buffer */

/* circular buffer */
/* size is whole multiplier of cr */
typedef struct {
    t_num *samples;     /* samples */
    t_num *curr;        /* current position */
    size_t dur;         /* length in cr */
    size_t count_to_last; /* number of blocks to end */
    size_t leng;        /* length in sr */
} t_buf2;

t_buf2 *dsp_alg_buf2_init(size_t block_size, size_t num_of_frames)
{
    t_buf2 *res = (t_buf2 *) malloc(sizeof(t_buf2));               /* M */
    res->dur = num_of_frames;
                                                                /* M */    
    res->samples = (t_num *) 
        calloc(2 * num_of_frames * block_size, sizeof(t_num));
    res->curr = res->samples;
    res->count_to_last = num_of_frames;
    res->leng = 2 * num_of_frames * block_size;

    return res;
}

/* init buffer
    dt -- length of the bufer in samples
 */
t_buf2 *dsp_alg_buf2_init_with_time(
    size_t sample_rate, size_t block_size, t_num dt) 
{
    return dsp_alg_buf2_init(block_size, 
        (size_t) (dt * (t_num) control_rate(sample_rate, block_size)));
}

void dsp_alg_buf2_free(t_buf2 *in)
{
    free(in->samples);                                          /* F */
    free(in);                                                   /* F */
}

size_t dsp_alg_buf2_length(t_buf2 *a)
{
    return a->leng;
}

/* write single block to buffer */
void dsp_alg_buf2w(size_t n, t_buf2 *st, const t_num *ina, const t_num *inb)
{
    if (st->count_to_last == 0) {
        st->curr = st->samples;
        st->count_to_last = st->dur;
    }

    t_num *curr = st->curr;
    while (n--) {
        *curr++ = *ina++;
        *curr++ = *inb++;
    }
    st->curr = curr;

    (st->count_to_last)--;
}

/* read single block from buffer */
void dsp_alg_buf2r(size_t n, t_buf2 *st, t_num *outa, t_num *outb)
{
    if (st->count_to_last == 0) {
        st->curr = st->samples;
        st->count_to_last = st->dur;
    }

    t_num *curr = st->curr;
    while (n--) {
        *outa++ = *curr++;
        *outb++ = *curr++;
    }
    st->curr = curr;

    (st->count_to_last)--;
}

void dsp_alg_buf2_trigger(t_buf *a, 
    void (*fn)(void *, size_t, t_num *), void *st)
{
    if (a->count_to_last == 0)
        fn(st, a->leng, a->samples);        
}

void dsp_alg_buf2_trigger_part(t_buf *a, 
    void (*fn)(void *, size_t, t_num *), void *st)
{
    fn(st, (a->curr - a->samples) / 2, a->samples);
}




/* double buffer with n channels */

/* writing to double buffer
 
   audio is written in one buffer while
   another process reads other buffer 
 */
typedef {
    t_num *buf0;
    t_num *buf1;
    t_num **ins;
    t_num *cur;
    size_t channels;
    size_t leng;
    size_t dur;
    size_t count_to_last;
    bool block0;
    bool block1;
    size_t to_read;
} t_dbufw;

t_dbufw *dsp_alg_dbufw_init(
    size_t block_size, size_t channels, size_t num_of_frames)
{
    t_dbufw *res = (t_dbufw *) malloc(sizeof(t_dbufw));       /* M */
    res->ins = (t_num **) malloc(channels * sizeof(t_num *));  /* M */

    res->leng = block_size * channels * num_of_frames;
    res->channels = channels;
    res->dur = num_of_frames;
    res->count_to_last = num_of_frames;
    res->block0 = true;
    res->block1 = false;
    res->to_read = 0;

    res->buf0 = (t_num *) calloc(res->leng, sizeof(t_num));     /* M */
    res->buf1 = (t_num *) calloc(res->leng, sizeof(t_num));     /* M */

    res->cur = buf0;

    return res;
}

void dsp_alg_dbufw_free(t_dbufw *a)
{
    free(a->ins);                                      /* F */
    free(a->buf0);                                      /* F */
    free(a->buf1);                                      /* F */
    free(a);                                            /* F */
}


void dsp_alg_dbufw(size_t n, t_dbufw *st, const t_num **ins)
{
    /* copy inputs */    
    int i;
    t_num **ins_copy = st->ins;
    for (i = 0; i < st->channels; i++) 
        *ins_copy++ = *ins++;

    /* when we finish to write onwe buffer
       we switch to another buffer and 
       toggle blocks */
    if (st->count_to_last == 0) {
        st->count_to_last = st->dur;        
        st->cur = (st->block0) ? st->buf2 : st->buf1;
        st->block0 = !(st->block0);
        st->block1 = !(st->block1);
    }

    t_num *cur = st->cur;
    while (n--)
        for (i = 0; i < st->channels; i++)    
            *cur++ = *(ins_copy[i])++;
    st->cur = cur;

    (st->count_to_last)--;
}


static void dsp_alg_dbufw_wait(t_dbufw *st)
{
    switch (st->to_read) {
        case 0: 
            while (st->block0);
            break;
        case 1:
            while (st->block1);
            break;
        default:
            break;    
    }
}


void dsp_alg_dbufw_read(t_dbufw *st, size_t leng, t_num *samples)
{
    dsp_alg_dbufw_wait(st);

}

