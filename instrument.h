#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "voice.h"


typedef struct{
    float volume;
    Voice voices[16];
    int active_voices;
    int attack;
    int decay;
    float sustain;
    int release;
    float pitch_bend;
} Instrument;

Instrument* create_instrument();

float instrument_render(Instrument* in, int time);
void instrument_note_on(Instrument* in, int pitch, float volume);
void instrument_deactivate_voice(Instrument* p, int index);

#endif
