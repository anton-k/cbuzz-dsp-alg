#ifndef DSP_ALG_BUFFER_H 
#define DSP_ALG_BUFFER_H 

#include "dsp-alg-types.h"

size_t dsp_alg_time_to_buf_length( const t_options *opt, t_num dur );

typedef struct {
    t_num     *samples;
    size_t    length;  
    size_t    dur;
    const t_options *opt;
} t_buf;

void dsp_alg_buf_init( 
    t_buf *res, const t_options *opt,
    size_t length, t_num *samples );

typedef struct {
    const t_buf *buf;
    size_t count_to_last;
    t_num  *cur;
} t_buf_seek;

typedef struct { t_buf_seek seek; } t_buf_read;
typedef struct { t_buf_seek seek; } t_buf_write;
typedef struct { t_buf_seek seek; } t_buf_append;

typedef struct {
    t_buf *buf;
    size_t count_to_last;
    t_buf_cbk cbk;
    void *data;
} t_buf_trigger;

typedef void (*t_buf_cbk)( size_t length, t_num *samples, void *data );

typedef struct { t_buf_trigger trigger; } t_buf_trigger_begin;
typedef struct { t_buf_trigger trigger; } t_buf_trigger_end;

void dsp_alg_buf_read_init( t_buf_read *res, t_buf *buf );
void dsp_alg_buf_read( t_buf_read *st, t_num *out );

void dsp_alg_buf_write_init( t_buf_write *res, t_buf *buf );
void dsp_alg_buf_write( t_buf_write *st, const t_num *in );

void dsp_alg_buf_append_init( t_buf_append *res, t_buf *buf );
void dsp_alg_buf_append( t_buf_append *st, const t_num *in );

void dsp_alg_buf_read_vdelay(t_buf_read *st, const t_num *in, t_num *out);
void dsp_alg_buf_trigger_begin_init( t_buf_trigger_begin *res, 
    t_buf *buf, t_buf_cbk cbk, void *data );

void dsp_alg_buf_trigger_begin( t_buf_trigger_begin *st );
void dsp_alg_buf_trigger_end_init( t_buf_trigger_begin *res, 
    t_buf *buf, t_buf_cbk cbk, void *data );
void dsp_alg_buf_trigger_end( t_buf_trigger_end *st );

#endif


