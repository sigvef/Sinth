#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sample.h"
#include "instrument.h"
#include "oscillators.h"

Instrument* create_instrument(const char*sample_filename){
    Instrument* instrument = (Instrument*) malloc(sizeof(Instrument));
    instrument->volume = 1;
    instrument->LP_buffer_tracker = 0;
    instrument->active_voices = 0;
    instrument->attack = 0;
    instrument->decay = 20000;
    instrument->sustain = 0.7f;
    instrument->release = 0;
    instrument->pitch_bend = 0.f;
    instrument->LFO_freq = -5;
    instrument->LFO_type = LFO_SIN;
    instrument->LFO_value = 0;
    instrument->LP_coeff[0] = 0.5f;
    instrument->LP_coeff[1] = 0.3f;
    instrument->LP_coeff[2] = 0.8f;
    //instrument->osc_type = oscillator_type;
    instrument->sample = load_sample(sample_filename);
    int i;
    for(i=0;i<16;i++){
        instrument->voices[i].on = 0;
    }
    for(i=0;i<LP_SIZE;i++){
        instrument->LP_buffer[i] = 0;
    }
    return instrument;
}


float instrument_render(Instrument* in, int time){

    float out = 0;

    switch(in->LFO_type){
        case LFO_SIN:
            in->LFO_value = osc_sin(in->LFO_freq, time);
            break;
        case LFO_SQU:
            in->LFO_value = osc_squ(in->LFO_freq, time);
            break;
        case LFO_SAW:
            in->LFO_value = osc_saw(in->LFO_freq, time);
            break;
        case LFO_TRI:
            in->LFO_value = osc_tri(in->LFO_freq, time);
            break;
        default:
            break;
    }

    int i;

    for(i=0;i<in->active_voices;i++){
        if(!in->voices[i].on){
            instrument_deactivate_voice(in,i);
            i--;continue;
        }
        in->voices[i].tracker++;
        float osc_value;
            osc_value = sample_render(in->sample,in->voices[i].pitch+in->pitch_bend, in->voices[i].tracker);
        out += in->voices[i].volume*osc_value;

    }

    return in->volume*out;
}

void instrument_note_on(Instrument* in, int pitch, int volume){
    if(in->active_voices < 16){
        in->voices[in->active_voices].pitch = pitch;
        in->voices[in->active_voices].volume = volume/512.f;
        in->voices[in->active_voices].on = 1;
        in->voices[in->active_voices].active = 1;
        in->voices[in->active_voices].tracker = 0;
        in->voices[in->active_voices].adsr_modifier = 0.f;
        in->active_voices++;
    }else{
        int i;
        int to_delete = 0;
        int lowest_volume = 1;
        for(i=0;i<16;i++){
            if(in->voices[i].volume*in->voices[i].adsr_modifier < lowest_volume){
                to_delete = i;
            }
        }
        instrument_deactivate_voice(in, to_delete);
        instrument_note_on(in,pitch,volume);
    }
}

void instrument_note_off(Instrument* in, int pitch){
    int i;
    for(i=0;i<in->active_voices;i++){
        if(in->voices[i].pitch == pitch){
             in->voices[i].on = 0;
             in->voices[i].tracker = 0;
        }
    }
}


void instrument_deactivate_voice(Instrument* in, int index){
    in->active_voices--;
    memcpy(&in->voices[index],&in->voices[in->active_voices],sizeof(Voice));

}
