#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#define LP_SIZE 8

#define LFO_SIN 0
#define LFO_SQU 1
#define LFO_SAW 2
#define LFO_TRI 3

#include "sample.h"
#include "voice.h"


typedef struct{
    Sample* sample;
    float volume;
    Voice voices[16];
    float LP_buffer[LP_SIZE];
    int LP_buffer_tracker;
    float LP_coeff[3];
    int active_voices;
    int attack;
    int decay;
    float sustain;
    int release;
    float pitch_bend;
    float LFO_freq;
    float LFO_value;
    int LFO_type;
    int osc_type;
    int t;
} Instrument;

Instrument* create_instrument(const char* sample_filename);

float instrument_render(Instrument* in, int time);
void instrument_note_on(Instrument* in, int pitch, int volume);
void instrument_note_off(Instrument* in, int pitch);
void instrument_deactivate_voice(Instrument* p, int index);

#endif
