#include <stdbool.h>
#include <stdlib.h>
#include <sndfile.h>

#include <dsp-alg-types.h>
#include <dsp-alg-buffer.h>


typedef SNDFILE t_sndfile;
typedef SF_INFO t_sf_info;

/* write files */

static void sndfilew_fn(void *vst, size_t n, t_num *samples);

typedef struct {
    t_sndfile *file;
    t_buf2 *buf;
    bool is_float;
} t_sndfilew;


t_sndfilew *dsp_alg_sndfilew_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames, int format)
{
    t_sndfilew *res = (t_sndfilew *) malloc(sizeof(t_sndfilew));  /* M */

    t_sf_info info;
    info.samplerate = sample_rate;
    info.channels = 2;
    info.format = format;

    res->file = sf_open(filename, SFM_WRITE, &info);
    res->buf  = dsp_alg_buf2_init(block_size, num_of_frames); /* hidden M */
    res->is_float = sizeof(t_num) == sizeof(float);

    return res;
}

void dsp_alg_sndfilew_free(t_sndfilew *st)
{
    dsp_alg_buf2_trigger_part(st->buf, sndfilew_fn, (void *) st);
    sf_close(st->file);
    dsp_alg_buf2_free(st->buf);                             /* F */
    free(st);                                               /* F */
}

void dsp_alg_sndfilew(size_t n, t_sndfilew *st, 
    const t_num *ina, const t_num *inb)
{
    dsp_alg_buf2_trigger(st->buf, sndfilew_fn, (void *) st);
    dsp_alg_buf2w(n, st->buf, ina, inb);
}

static void sndfilew_fn(void *vst, size_t n, t_num *samples)
{
    t_sndfilew *st = (t_sndfilew *) vst;

    if (st->is_float)
        sf_write_float(st->file, (float *) samples, n);
    else
        sf_write_double(st->file, (double *) samples, n);        
}


t_sndfilew *dsp_alg_wav16w_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames)
{
    return dsp_alg_sndfilew_init(sample_rate, block_size, 
            filename, num_of_frames,
            SF_FORMAT_WAV | SF_FORMAT_PCM_16);
}

t_sndfilew *dsp_alg_wav24w_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames)
{
    return dsp_alg_sndfilew_init(sample_rate, block_size, 
            filename, num_of_frames,
            SF_FORMAT_WAV | SF_FORMAT_PCM_24);
}

t_sndfilew *dsp_alg_wav32w_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames)
{
    return dsp_alg_sndfilew_init(sample_rate, block_size, 
            filename, num_of_frames,
            SF_FORMAT_WAV | SF_FORMAT_PCM_32);
}

t_sndfilew *dsp_alg_aiff16w_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames)
{
    return dsp_alg_sndfilew_init(sample_rate, block_size, 
            filename, num_of_frames,
            SF_FORMAT_AIFF | SF_FORMAT_PCM_16);
}

t_sndfilew *dsp_alg_aiff24w_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames)
{
    return dsp_alg_sndfilew_init(sample_rate, block_size, 
            filename, num_of_frames,
            SF_FORMAT_AIFF | SF_FORMAT_PCM_24);
}

t_sndfilew *dsp_alg_aiff32w_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames)
{
    return dsp_alg_sndfilew_init(sample_rate, block_size, 
            filename, num_of_frames,
            SF_FORMAT_AIFF | SF_FORMAT_PCM_32);
}

/* ------------------------------------------------- */
/* read files */

typedef struct {
    t_sndfile *file;
    t_buf2 *out_buf;
    t_num  *in_buf;
    size_t leng;
    size_t channels;

    size_t int_ratio;
    t_num  num_ratio;

    bool is_same_rate;
    bool is_integer_rate;
    bool is_float;
    bool is_cycle;
} t_snfiler;

t_sndfiler *dsp_alg_sndfiler_init(size_t sample_rate, size_t block_size,
    const char *filename, size_t num_of_frames, bool is_cycle)
{
    t_sndfiler *res = (t_sndfiler *) malloc(sizeof(t_sndfiler));    /* M */

    t_sf_info info;
    res->file = sf_open(filename, SFM_READ, &info);

    res->is_same_rate = sample_rate == info.samplerate;
    res->is_integer_rate = (info.samplerate % sample_rate) == 0;
    res->is_float = sizeof(t_num) == sizeof(float);
    res->is_cycle = is_cycle;

    res->int_ratio = (res->is_integer_rate) 
        ? info.samplerate / sample_rate : 0;
    res->num_ratio = (t_num) info.samplerate / (t_num) sample_rate;

    res->channels = info.channels;
    res->out_buf = dsp_alg_buf2_init(block_size, num_of_frames); /* hidden M */

    /* plus one sample for possible linear interpolation */
    res->leng = (info.channels * 1) + (size_t) (
        (t_num) dsp_alg_buf2_length(res->out_buf) * res->num_ratio);
    res->in_buf = (t_num *) calloc(res->leng, sizeof(t_num));       /* M */

    return res;
}

void dsp_alg_sndfiler_free(t_sndfiler *a)
{
    sf_close(a->file);
    free(a->in_buf);                                                /* F */
    dsp_alg_buf2_free(a->out_buf);                                  /* F */
    free(a);                                                        /* F */
}


static void sndfiler_fn(void *vst, size_t n, t_num *samples);

void dsp_alg_sndfiler(size_t n, t_sndfiler *st, t_num *outa, t_num *outb)
{
    dsp_alg_buf2_trigger_start(st->out_buf, sndfiler_fn, (void *) st);
    dsp_alg_buf2r(n, st->out_buf, outa, outb);
}


static void sndfiler_same_rate_fn(size_t n, size_t channels, 
    t_num )

static void sndfiler_mono_fn(t_sndfiler *st, size_t n, t_num *samples)
{
        
}

static void sndfiler_stereo_fn(t_sndfiler *st, size_t n, t_num *samples)
{

}

static void sndfiler_more_fn(t_sndfiler *st, size_t n, t_num *samples)
{

}

static void sndfiler_fn(void *vst, size_t n, t_num *samples)
{
    t_sndfiler *st = (t_sndfiler *) vst;

    if (st->channels == 1)
        sndfiler_mono_fn(st, n, samples);
    else if (st->channels == 2)
        sndfiler_stereo_fn(st, n, samples);
    else
        sndfiler_more_fn(st, n, samples);
}







