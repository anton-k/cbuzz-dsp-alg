#include <stdlib.h>

#include <portaudio.h>

#include <dsp-alg-types.h>
#include <dsp-alg-buffer.h>

size_t dsp_alg_device_output_channels(size_t device)
{
    return Pa_GetDeviceInfo(device)->maxOutputChannels;
}

size_t dsp_alg_device_input_channels(size_t device)
{
    return Pa_GetDeviceInfo(device)->maxInputChannels;
}

typedef struct {
    t_dbufw *buf;
    PaStream *stream;
} t_paw;

static PaStreamCallback pa_write_callback;

t_paw *dsp_alg_paw_init(size_t sample_rate, size_t block_size,
    size_t device, size_t num_of_frames)
{
    t_paw *res = (t_paw *) malloc(sizeof(t_paw));               /* M */

    PaStreamParameters out_params;
    out_params.device = device;             
    out_params.channelCount = dsp_alg_device_output_channels(device);
    out_params.sampleFormat = paFloat32;
    out_params.suggestedLatency =                               
        Pa_GetDeviceInfo(device)->defaultOutputLatency;
    out_params.hostApiSpecificStreamInfo = NULL;
 
    res->stream = stream;
    res->buf = dsp_alg_dbufw_init(block_size,                   /* hidden M */
        out_params.channelCount, num_of_frames);

    Pa_OpenStream(
            &(res->stream),
            NULL, /* no input */
            &out_params,
            sample_rate,
            dsp_alg_dbufw_length(res->buf),
            paNoFlags,
            pa_write_callback,
            (void *) res);   

    Pa_StartStream(res->stream);

    return res;
}

void dsp_alg_paw_free(t_paw *a)
{
    Pa_StopStream(a->stream);
    Pa_CloseStream(a->stream);
    dsp_alg_dbufw_free(a->buf);                                 /* F */
    free(a);                                                    /* F */
}

void dsp_alg_paw(size_t n, t_paw *st, const t_num **ins)
{
    dsp_alg_dbufw(n, st, ins);
}

static int pa_write_callback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData )
{
    t_paw *st = (t_paw *) userData;
    
    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;

    dsp_alg_dbufw_read(st->buf, framesPerBuffer, outputBuffer);
    dsp_alg_dbufw_swap(st->buf);
}

size_t dsp_alg_paw_channels(t_paw *a)
{
    return dsp_alg_dbufw_channels(a->buf);
}

/*
typedef struct {

} t_par;

t_par *dsp_alg_par_init(size_t sample_rate, size_t block_size,
    size_t device, size_t num_of_frames);

void dsp_alg_par_free(t_par *);
void dsp_alg_par(size_t n, t_par *st, t_num **outs);

size_t dsp_alg_par_channels(t_par *)
{
    return dsp_alg_dbufr_channels(a->buf);
}
*/

