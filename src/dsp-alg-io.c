#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sndfile.h>
#include <assert.h>

#include <dsp-alg-types.h>
#include <dsp-alg-buffer.h>
#include <dsp-alg-num.h>

/* maybe whole idea of reading with specific buffer reader position
 * is flawed. consider better solution, just
 * read/write to buffers and let the patcher layer
 * decide when to read/write */

static size_t printf_buf_length = 128;
static void sig_printf_fn(void *vst, size_t leng, t_num *in);

typedef struct {
    char *label;
    t_buf *buf;
    size_t bs;
} t_sig_printf;

static char *string_copy(const char *str)
{
    char *res = (char *) malloc((1+strlen(str))*sizeof(char));    /* M */
    strcpy(res, str);
    return res;    
}

t_sig_printf *dsp_alg_sig_printf_init(size_t block_size, const char *label)
{
    t_sig_printf *res = (t_sig_printf *) malloc(sizeof(t_sig_printf)); /* M */
    res->label = string_copy(label);                          /* hidden M */
    res->buf   = dsp_alg_buf_init(block_size,                 /* hidden M */  
        printf_buf_length / block_size);
    res->bs    = block_size;

    return res;
}

void dsp_alg_sig_printf_free(t_sig_printf *st)
{
    dsp_alg_buf_trigger_part(st->buf, sig_printf_fn, (void *)st);
    free(st->label);                                        /* F */
    dsp_alg_buf_free(st->buf);                              /* F */
    free(st);                                               /* F */
}

void dsp_alg_sig_printf(size_t n, t_sig_printf *st, const t_num *in) 
{
    dsp_alg_bufw(n, st->buf, in);
    dsp_alg_buf_trigger(st->buf, sig_printf_fn, (void *) st);
}

void dsp_alg_set_sig_printf_buf_length(size_t val)
{
    printf_buf_length = val;
}

static void sig_printf_fn(void *vst, size_t leng, t_num *in)
{
    t_sig_printf *st = (t_sig_printf *) vst;
    size_t bs = st->bs;

    int i;
    printf("%s: ", st->label);
    for (i = 0; i < leng; i++) {
        if (i % bs == 0 && i != 0) 
            printf("\n%s: ", st->label);

        printf("%.2f", *in++);
        if (i % bs != 0)
            printf(", ");
    }
    printf("\n");
}

static size_t fprintf_buf_length = 2048;
static void sig_fprintf_fn(void *vst, size_t n, t_num *in);

typedef struct {
    FILE *file;
    t_buf *buf;
    char *divisor;
    bool is_first_invocation;
} t_sig_fprintf;

t_sig_fprintf *dsp_alg_sig_fprintf_init(
    size_t block_size, const char *filename, const char *divisor)
{
     t_sig_fprintf *res = 
         (t_sig_fprintf *) malloc(sizeof(t_sig_fprintf));         /* M */

     res->file = fopen(filename, "w");
     res->divisor = string_copy(divisor);                       /* hidden M */
     res->buf = dsp_alg_buf_init(block_size,                    /* hidden M */
             fprintf_buf_length / block_size);
     res->is_first_invocation = true;

     return res;
}

void dsp_alg_sig_fprintf_free(t_sig_fprintf *st)
{
    /* write last samples */
    dsp_alg_buf_trigger_part(st->buf, sig_fprintf_fn, (void *)st);

    free(st->divisor);                                          /* F */
    fclose(st->file);       
    dsp_alg_buf_free(st->buf);                                  /* F */
    free(st);                                                   /* F */
}

/* when buffer is full we write to disk */
void dsp_alg_sig_fprintf(size_t n, t_sig_fprintf *st, const t_num *in)
{
    dsp_alg_bufw(n, st->buf, in);
    dsp_alg_buf_trigger(st->buf, sig_fprintf_fn, (void *) st);
}

void dsp_alg_set_sig_fprintf_buf_length(size_t val)
{
    fprintf_buf_length = val;
}


static void sig_fprintf_fn(void *vst, size_t n, t_num *in) 
{
    t_sig_fprintf *st = (t_sig_fprintf *) vst;
    
    if (!st->is_first_invocation) 
        fprintf(st->file, "%s", st->divisor);
    else
        st->is_first_invocation = false;    

    while (n--) {    
        fprintf(st->file, "%f", (double) *in++);
        if (n > 0)
            fprintf(st->file, "%s", st->divisor);
    }
}


typedef SNDFILE t_sndfile;
typedef SF_INFO t_sf_info;

static wavw_buf_length = 2048;
static void wavw_fn(void *vst, size_t n, t_num *samples);

typedef struct {
    t_sndfile *file;
    t_buf2 *buf;
    bool is_float;
} t_wavw;

t_wavw *dsp_alg_wavw_init(size_t sample_rate, size_t block_size,
    const char *filename)
{
    t_wavw *res = (t_wavw *) malloc(sizeof(t_wavw));        /* M */
    t_sf_info *info = (t_sf_info *) malloc(sizeof(t_sf_info));  /* M */

    info->samplerate = sample_rate;
    info->channels = 2;
    info->format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    res->file = sf_open(filename, SFM_WRITE, info);
    res->buf  = dsp_alg_buf2_init(block_size, wavw_buf_length); /* hidden M */
    res->is_float = sizeof(t_num) == sizeof(float);

    free(info);                                             /* F */
    return res;
}

void dsp_alg_wavw_free(t_wavw *st)
{
    dsp_alg_buf2_trigger_part(st->buf, wavw_fn, (void *) st);
    sf_close(st->file);
    dsp_alg_buf2_free(st->buf);                             /* F */
    free(st);                                               /* F */
}

void dsp_alg_wavw(size_t n, t_wavw *st, const t_num *ina, const t_num *inb)
{
    dsp_alg_buf2w(n, st->buf, ina, inb);
    dsp_alg_buf2_trigger(st->buf, wavw_fn, (void *) st);
}


void dsp_alg_set_wavw_buf_length(size_t val)
{
    wavw_buf_length = val;
}

static void wavw_fn(void *vst, size_t n, t_num *samples)
{
    t_wavw *st = (t_wavw *) vst;

    if (st->is_float)
        sf_write_float(st->file, (float *) samples, n);
    else
        sf_write_double(st->file, (double *) samples, n);        
}



typedef struct {
    t_sndfile *file;
    t_num ratio;
    t_num *samples;
    bool is_mono;
    bool is_same_rate;
    bool is_float;
    size_t num_of_frames;
    size_t leng;
    t_buf2 *buf;
} t_wavr;


t_wavr *dsp_alg_wavr_init(size_t sample_rate, 
    const char *filename, t_buf2 *buf)
{
    size_t num_of_frames = dsp_alg_buf2_length(buf);

    t_wavr *res = (t_wavr *) malloc(sizeof(t_wavr));        /* M */
    t_sf_info *info = (t_sf_info *) malloc(sizeof(t_sf_info));  /* M */
    
    res->file = sf_open(filename, SFM_READ, info);
    res->is_mono = (info->channels == 1);
    res->is_same_rate = (info->samplerate == sample_rate);

    assert(info->channels == 1 || info->channels == 2);
    if (res->is_same_rate) { 
        res->ratio = 1.0;
        res->leng = 2 * num_of_frames;
    }
    else {
        res->ratio = (t_num) sample_rate / (t_num) info->samplerate;
        res->leng = (size_t) (2.0 * (t_num) num_of_frames / res->ratio);
    }
                                                            /* M */
    res->samples = (t_num *) calloc(res->leng, sizeof(t_num));
    res->buf = buf;
    res->is_float = sizeof(t_num) == sizeof(float);
    res->num_of_frames = num_of_frames;

    free(info);                                             /* F */
    return res;
}

void dsp_alg_wavr_free(t_wavr *st)
{
    sf_close(st->file);
    free(st->samples);                                      /* F */
    free(st);                                               /* F */
}




/*
static void wavr_fn_mono(t_wavr *st, size_t n, t_num *out)
{
}

static void wavr_fn_stereo(t_wavr *st, size_t n, t_num *out)
{
}

void wavr_fn(void *vst, size_t n, t_num *out)
{
    t_wavr *st = (t_wavr *) vst;
    
    if (st->is_float)
        sf_read_float(st->file, (float *) st->samples, st->num_of_frames);
    else
        sf_read_double(st->file, (double *) st->samples, st->num_of_frames);

    if (st->is_mono)  
        wavr_fn_mono(st, n, out);
    else
        wavr_fn_stereo(st, n, out);
}

void wavr(t_wavr *st)
{
    dsl_alg_buf2_trigger(st->buf, wavr_fn, (void *) st);
}
*/
