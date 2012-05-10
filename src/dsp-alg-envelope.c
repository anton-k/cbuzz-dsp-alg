#include <stdlib.h>
#include <math.h>
#include <queue.h>

#include <dsp-alg-types.h>
#include <time.h>
/* TODO rewrite linseg with queue
   all units have to run in constant space,
   but linseg calls free/malloc inside working cycle
    solution: switch from queue to circle buffer,
              linseg will contain limited number of segments
*/

/* hold  */

void dsp_alg_hold(size_t n, t_num cur, t_num* out)
{
    while (n--)
        *out++ = cur;
}

/* line */

void dsp_alg_line(size_t n, t_num incr, t_num *cur, t_num *out)
{
    while (n--) {
        *out++ = *cur;
        *cur += incr;
    }
}

/* expline */

void dsp_alg_expline(size_t n, t_num incr, t_num *cur, t_num *out)
{
    while (n--) {
        *out++ = *cur;
        *cur *= incr;
    }
}


/* segments */


/* There is assumption that breackpoints live in
	control rate, so each segment contains at least
	block_size of points, and "dur" is count in 
	control rate ticks !not sample rate! */
typedef struct{
	t_num incr;		/* increment */
	t_num base;		/* base point */
	t_time dur;		/* duration in control rate ticks */
} t_seg;

typedef Queue t_seg_queue;

t_seg_queue *seg_queue_init();
void seg_queue_free(t_seg_queue *q);
bool seg_queue_is_empty(t_seg_queue *q);
t_seg *seg_queue_peek(t_seg_queue *);
void seg_queue_pop(t_seg_queue *);
void seg_queue_push(t_seg_queue *, t_seg *);


typedef struct {
    t_seg_queue *segs;
    t_num last_value;
    size_t sample_rate;
    size_t control_rate;
} t_linseg;

typedef t_linseg t_expseg;

static t_linseg *seg_init(size_t sample_rate, size_t block_size, t_num last);
static void seg_free(t_linseg *st);
static void seg_reset(t_linseg *st);
static void seg_append(
    t_num (*get_incr)(t_num, t_num, t_num),
    t_linseg *st,         
    t_num dur, t_num last);

static void seg_tick(
        void (*seg_run)(size_t, t_num, t_num *, t_num *),
        size_t n, t_linseg *st, t_num *out);


static t_num linseg_incr(t_num p0, t_num dt, t_num p1);
static t_num expseg_incr(t_num p0, t_num dt, t_num p1);

t_linseg *dsp_alg_linseg_init(size_t sample_rate, size_t block_size)
{
    return seg_init(sample_rate, block_size, 0.0);
}

void dsp_alg_linseg_free(t_linseg *st)
{
    seg_free(st);
}

t_expseg *dsp_alg_expseg_init(size_t sample_rate, size_t block_size)
{
    return (t_expseg *) seg_init(sample_rate, block_size, 0.00001);
}

void dsp_alg_expseg_free(t_expseg *st)
{
    seg_free((t_linseg *) st);
} 

void dsp_alg_linseg_reset(t_linseg *st)
{
    seg_reset(st);
}

void dsp_alg_expseg_reset(t_expseg *st)
{
    seg_reset((t_expseg *) st);    
}

void dsp_alg_linseg_append(t_linseg *st,
        t_num dur, t_num last)
{
    seg_append(linseg_incr, st, dur, last);
}

void dsp_alg_expseg_append(t_expseg *st,
        t_num dur, t_num last)
{
    seg_append(expseg_incr, (t_linseg *) st, dur, last);
}

void dsp_alg_linseg(size_t n, t_linseg *st, t_num *out)
{
    seg_tick(dsp_alg_line, n, st, out);
}


void dsp_alg_expseg(size_t n, t_expseg *st, t_num *out)
{
    seg_tick(dsp_alg_expline, n, st, out);
}

/* ------------------------------------------------------ */
/* increments */

static t_num linseg_incr(t_num p0, t_num dt, t_num p1)
{
    return (p1 - p0)/dt;
}

static t_num expseg_incr(t_num p0, t_num dt, t_num p1)
{
    return pow(p1/p0, 1.0/dt);
}


/* generic segment */

static t_linseg *seg_init(size_t sample_rate, size_t block_size, t_num last)
{
    t_linseg *res = (t_linseg *) malloc(sizeof(t_linseg));
    res->segs = seg_queue_init();
    res->last_value = last;
    res->sample_rate = sample_rate;
    res->control_rate = sample_rate / block_size;
    return res;
}

static void seg_free(t_linseg *st)
{
    seg_queue_free(st->segs);
    free(st);
}


/* last value is set to the current value */
static void seg_reset(t_linseg *st)
{
    if (!seg_queue_is_empty(st->segs)) 
        st->last_value = (seg_queue_peek(st->segs))->base;
    
    seg_queue_free(st->segs);
    st->segs = seg_queue_init();
}

static void seg_append(
        t_num (*get_incr)(t_num, t_num, t_num),
        t_linseg *st, t_num dur, t_num last)
{
    t_num dn_rem = (t_num) st->control_rate * (dur - (size_t) dur);
    t_time dn = time_init((size_t) dur, (size_t) dn_rem, st->control_rate);
    
    if (time_is_zero(dn)) {
        st->last_value = last;
    }
    else {
        t_seg *seg = (t_seg *) malloc(sizeof(t_seg));
        t_num p0 = st->last_value;
        t_num p1 = last;
        
        seg->base = st->last_value;
        seg->incr = get_incr(p0, (t_num)(dur * st->sample_rate), p1);
        seg->dur  = dn;

        seg_queue_push(st->segs, seg);
        st->last_value = last;
    }
}

static void seg_tick(
        void (*seg_run)(size_t, t_num, t_num *, t_num *),
        size_t n, t_linseg *st, t_num *out)
{
    if (seg_queue_is_empty(st->segs)) {
        dsp_alg_hold(n, st->last_value, out);
    }
    else {
        t_seg *seg = seg_queue_peek(st->segs);

        if (time_is_zero(seg->dur)) {
            seg_queue_pop(st->segs);
            seg_tick(seg_run, n, st, out);
        }
        else {
            time_pred(&seg->dur);
            seg_run(n, seg->incr, &seg->base, out);
        }
    }
}

/* ----------------------------------------------------- */
/* data structures */

/* segments queue */

typedef Queue seg_queue;

t_seg_queue *seg_queue_init()
{
    return (t_seg_queue *) queue_new();
}

void seg_queue_free(t_seg_queue *q)
{
    while (!seg_queue_is_empty(q))
        seg_queue_pop(q);
    
    queue_free(q);
}

bool seg_queue_is_empty(t_seg_queue *q)
{
    return queue_is_empty(q);
}

t_seg *seg_queue_peek(t_seg_queue *q) 
{
    return (t_seg *) queue_peek_head(q);
}

void seg_queue_pop(t_seg_queue *q) 
{
    free(queue_peek_head(q));
    queue_pop_head(q);
}

void seg_queue_push(t_seg_queue *q, t_seg *a)
{
    queue_push_tail(q, (void *) a);
}



