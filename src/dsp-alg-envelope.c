#include <math.h>
#include <dsp-alg-envelope.h>

/* ------------------------------------------------ */
/* segment api */

static void seg_init( t_seg *res, const t_options *opt, double x0 );
static void seg( t_seg *st, void (*run)(t_seg *, t_num *), t_num *out );
static void seg_set( t_seg *st, 
    t_num (*get_incr)( t_num p0, t_num dt, t_num p1),
    t_num dur, t_num next_val );

/* basic shapes */
static void hold(t_seg *st, t_num* out);
static void line(t_seg *st, t_num *out);
static void exponent(t_seg *st, t_num *out);

/* increments */
static t_num line_incr(t_num p0, t_num dt, t_num p1);
static t_num exponent_incr(t_num p0, t_num dt, t_num p1);

/* duration managment */
static bool seg_is_over( const t_seg *a );
static void seg_decrement_dur( t_seg *a );
static void seg_set_dur( t_seg *a, t_num dur );

/* ------------------------------------------------ */
/* linear segments */

void dsp_alg_linseg_init( t_linseg *res, const t_options *opt, double x0 )
{
    seg_init(&(res->seg), opt, x0);
}

void dsp_alg_linseg( t_linseg *st, t_num *out )
{
    seg( &(st->seg), line, out );
}

void dsp_alg_linseg_set( t_linseg *st, t_num dur, t_num next_val )
{
    seg_set( &(st->seg), line_incr, dur, next_val );
}

/* exponential segments */


void dsp_alg_expseg_init( t_expseg *res, const t_options *opt, double x0 )
{
    seg_init(&(res->seg), opt, x0);
}

void dsp_alg_expseg( t_expseg *st, t_num *out )
{
    seg( &(st->seg), exponent, out );
}

void dsp_alg_expseg_set( t_expseg *st, t_num dur, t_num next_val )
{
    seg_set( &(st->seg), exponent_incr, dur, next_val );
}

/* --------------------------------------------------------------- */
/* low level segments */

static void seg_init( t_seg *res, const t_options *opt, double x0 )
{
    res->base = x0;
    res->incr = 0;
    res->dur  = time_zero(opt->cr);
    res->opt  = opt;
}

static void seg( t_seg *st, void (*run)(t_seg *, t_num *), t_num *out )
{
    if ( seg_is_over(st) ) {
        hold( st, out );
    } else {
        run( st, out );
        seg_decrement_dur( st );
    }
}

static void seg_set( t_seg *st, 
    t_num (*get_incr)( t_num p0, t_num dt, t_num p1),
    t_num dur, t_num next_val )
{
    st->incr = get_incr(st->base, (t_num) st->opt->sr * dur , next_val);
    seg_set_dur(st, dur);
}

/* ---------------------------------------------------- */
/* segments */
 
/* hold  */

static void hold(t_seg *st, t_num* out)
{
    size_t n = st->opt->bs;
    const t_num cur = st->base;

    while (n--)
        *out++ = cur;
}

/* line */

static void line(t_seg *st, t_num *out)
{
    size_t n = st->opt->bs;
    const t_num incr = st->incr;
    t_num *cur = &(st->base);

    while (n--) {
        *out++ = *cur;
        *cur += incr;
    }
}

/* expline */

static void exponent(t_seg *st, t_num *out)
{
    size_t n = st->opt->bs;
    const t_num incr = st->incr;
    t_num *cur = &(st->base);

    while (n--) {
        *out++ = *cur;
        *cur *= incr;
    }
}
/* ------------------------------------------------------ */
/* increments */

static t_num line_incr(t_num p0, t_num dt, t_num p1)
{
    return (p1 - p0)/dt;
}

static t_num exponent_incr(t_num p0, t_num dt, t_num p1)
{
    return pow(p1/p0, 1.0/dt);
}

/* ------------------------------------------------------ */
/* duration managment */

static bool seg_is_over( const t_seg *a )
{
    return time_is_zero(a->dur);
}

static void seg_decrement_dur( t_seg *a )
{
    time_pred(&(a->dur));
}

static void seg_set_dur( t_seg *a, t_num dur )
{
    t_num dn_rem = (t_num) a->opt->cr * (dur - (size_t) dur);
    a->dur = time_init((size_t) dur, (size_t) dn_rem, a->opt->cr);    
}

