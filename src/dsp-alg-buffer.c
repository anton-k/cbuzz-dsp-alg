#include <dsp-alg-buffer.h>

#define BUF_SEEK_BS(a)  (a->seek->buf->opt->bs)
#define BUF_SEEK_CUR(a) (a->seek.cur)

/* how many samples do you need to store the `dur` seconds of sound */
size_t dsp_alg_time_to_buf_length( const t_options *opt, t_num dur )
{
    return (size_t) (dur * opt->cr) * opt->bs;
}

void dsp_alg_buf_init( 
    t_buf *res, const t_options *opt,
    size_t length, t_num *samples )
{
    const size_t dur = length / opt->bs;

    res->opt        = opt;
    res->samples    = samples;
    res->length     = dur * opt->bs;
    res->dur        = dur;
}


static void buf_seek_init( t_buf_seek *res, t_buf *buf ) 
{
    res->buf = buf;
    res->count_to_last = buf->dur;
    res->cur = buf->samples;
}

static void buf_seek_move( t_buf_seek *a )
{
    a->cur += a->buf->opt->bs;
    (a->count_to_last)--;

    if (a->count_to_last <= 0) {
        a->cur = a->buf->samples;
        a->count_to_last = a->buf->dur;
    }
}

static void buf_trigger_init( t_buf_trigger *res, t_buf *buf, 
    t_buf_cbk cbk, void *data)
{
    res->buf = buf;
    res->count_to_last = buf->dur;
    res->cbk = cbk;
    res->data = data;
}

static void buf_trigger_fire( const t_buf_trigger *a )
{
    (a->cbk)( a->buf->length, a->buf->samples, a->data );
}

static bool buf_trigger_is_begin( const t_buf_trigger *a )
{
    a->count_to_last == a->buf->dur;
}

static bool buf_trigger_is_end( const t_buf_trigger *a )
{
    a->count_to_last == 0;
}

static void buf_trigger( t_buf_trigger *st, 
    bool (*is_time)( const t_buf_trigger *) )
{
    (st->count_to_last)--;

    if ( is_time(st) )
        buf_trigger_fire( st );
    
    if (a->count_to_last <= 0) 
        a->count_to_last = a->buf->dur;    
}

void dsp_alg_buf_read_init( t_buf_read *res, t_buf *buf )
{
    buf_seek_init( &(res->seek), buf );
}

void dsp_alg_buf_read( t_buf_read *st, t_num *out )
{
    size_t n    = BUF_SEEK_BS(st);
    t_num *cur  = BUF_SEEK_CUR(st);
    while (n--)    
        *out++ = *cur++;

    buf_seek_move( &(st->seek) );
}
    

void dsp_alg_buf_write_init( t_buf_write *res, t_buf *buf )
{
    buf_seek_init( &(res->seek), buf );
}

void dsp_alg_buf_write( t_buf_write *st, const t_num *in )
{
    size_t n    = BUF_SEEK_BS(st);
    t_num *cur  = BUF_SEEK_CUR(st);
    while (n--)    
        *cur++ = *in++;

    buf_seek_move( &(st->seek) );    
}

void dsp_alg_buf_append_init( t_buf_append *res, t_buf *buf )
{
    buf_seek_init( &(res->seek), buf );
}


void dsp_alg_buf_append( t_buf_append *st, const t_num *in )
{
    size_t n    = BUF_SEEK_BS(st);
    t_num *cur  = BUF_SEEK_CUR(st);
    while (n--)    
        *cur++ += *in++;

    buf_seek_move( &(st->seek) );    

    
void dsp_alg_buf_read_vdelay(t_buf_read *st, const t_num *in, t_num *out)
{
    t_num y0, y1;
    t_num x, fx, dx;    
    t_num *x0, *x1;

    size_t n   = BUF_SEEK_BS(st);
    t_num *cur = BUF_SEEK_CUR(st);
    while (n--) {
       x = *in++;
       fx = (t_num) floor((double) x);
       
       while (fx >= st->leng)
           fx -= st->leng;

       dx = x - fx;
       x0 = cur - (int) fx;

       while (x0 < st->samples) 
           x0 += st->leng;       

       x1 = (x0 - st->samples < st->leng - 1) ? x0+1 : st->samples;
       y0 = *x0;
       y1 = *x1;

       *out++ = y0 + (y1 - y0) * dx;
       cur++;
    }

    buf_seek_move( &(st->seek) );
}


void dsp_alg_buf_trigger_begin_init( t_buf_trigger_begin *res, 
    t_buf *buf, t_buf_cbk cbk, void *data )
{
    buf_trigger_init( &(res->trigger), buf, cbk, data );
}

void dsp_alg_buf_trigger_begin( t_buf_trigger_begin *st )
{
    buf_trigger( &(st->trigger), buf_trigger_is_begin );
}

void dsp_alg_buf_trigger_end_init( t_buf_trigger_begin *res, 
    t_buf *buf, t_buf_cbk cbk, void *data )
{
    buf_trigger_init( &(res->trigger), buf, cbk, data );            
}

void dsp_alg_buf_trigger_end( t_buf_trigger_end *st )
{
    buf_trigger( &(st->trigger), buf_trigger_is_end );
}


