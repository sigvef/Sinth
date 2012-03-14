#include "mixer.h"

Mixer* create_mixer(){
    Mixer* mixer = (Mixer*) malloc(sizeof(Mixer));
    mixer->number_of_instruments = 0;
    mixer->volume =1/64.f;
    return mixer;
}


short mixer_render(Mixer* m, int time){
    float out = 0;
    int i;
    for(i=0;i<m->number_of_instruments;i++){
        out += instrument_render(m->instruments[i], time);
    }

    return (short)((sizeof(short)*8.f)/2.f+m->volume*(2<<(sizeof(short)*8))*out);
}

void mixer_add_instrument(Mixer* m, Instrument* in){
    if(m->number_of_instruments < 16){
        m->instruments[m->number_of_instruments++] = in;
    }
}
