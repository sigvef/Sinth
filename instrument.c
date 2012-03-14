#include "instrument.h"
#include "oscillators.h"

Instrument* create_instrument(){
    Instrument* instrument = (Instrument*) malloc(sizeof(Instrument));
    instrument->volume = 1;
    instrument->active_voices = 0;
    instrument->attack = 1;
    instrument->decay = 20000;
    instrument->sustain = 0.6f;
    instrument->release = 3000;
    instrument->pitch_bend = 0.f;
    int i;
    for(i=0;i<16;i++){
        instrument->voices[i].on = 0;
    }
    return instrument;
}


float instrument_render(Instrument* in, int time){

    float out = 0;

    int i;

    for(i=0;i<in->active_voices;i++){
        float adsr_modifier = 0.f;

        if(in->voices[i].on){
            if(in->voices[i].tracker <= in->attack){
                in->voices[i].adsr_modifier = (float)(in->voices[i].tracker)/in->attack;
                in->voices[i].tracker++;
            }
            else if(in->voices[i].tracker <= in->attack + in->decay){
                in->voices[i].adsr_modifier = 1 - (float)((1-in->sustain)*(in->voices[i].tracker - in->attack))/in->decay;
                in->voices[i].tracker++;
            }else{
                in->voices[i].adsr_modifier = in->sustain;
            }
            adsr_modifier = in->voices[i].adsr_modifier;
        } else{
            adsr_modifier = in->voices[i].adsr_modifier * (1-(float)(in->voices[i].tracker)/in->release);
            in->voices[i].tracker++;

            if(adsr_modifier <= 0){
                instrument_deactivate_voice(in,i);
                i--;continue;
            }
        }


        out += adsr_modifier*in->voices[i].volume*osc_squ(in->voices[i].pitch+in->pitch_bend, time);



    }
    return out;
}

void instrument_note_on(Instrument* in, int pitch, float volume){
    if(in->active_voices < 16){
        in->voices[in->active_voices].pitch = pitch;
        in->voices[in->active_voices].volume = volume/128.f;
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
