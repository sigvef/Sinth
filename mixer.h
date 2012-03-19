#ifndef MIXER_H
#define MIXER_H

#include "instrument.h"

typedef struct{
    float volume;
    Instrument* instruments[16];
    int number_of_instruments;
    short reverb_buffer[44100];
    int reverb_buffer_tracker;
} Mixer;

Mixer* create_mixer();

short mixer_render(Mixer* m, int time);
void mixer_add_instrument(Mixer* m, Instrument* in);

#endif
