#ifndef DSP_ALG_MIDI_H
#define DSP_ALG_MIDI_H

#include <dsp-alg-types.h>

typedef struct _midi t_midi;

t_midi *dsp_alg_midi_init(size_t channel_id);
void dsp_alg_midi_free(t_dac *);

typedef struct _midi_note t_midi_note;

/* ??? need for events or midi notes */
void dsp_alg_midiw(size_t n, t_midi *st, const t_midi_note *in);
void dsp_alg_midir(size_t n, t_midi *st, t_midi_note *out);

#endif


