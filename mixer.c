#include "mixer.h"
#include <math.h>

Mixer* create_mixer(){
    Mixer* mixer = (Mixer*) malloc(sizeof(Mixer));
    mixer->number_of_instruments = 0;
    mixer->volume =1/128.f;
    mixer->reverb_buffer_tracker = 0;
    int i;
    for(i=0;i<44100;i++){
        mixer->reverb_buffer[i] = 0;
    }
    return mixer;
}


short mixer_render(Mixer* m, int time){
    float out = 0;
    int i;
    for(i=0;i<m->number_of_instruments;i++){
        out += instrument_render(m->instruments[i], time);
    }
    short return_value = m->volume*(2<<(sizeof(short)*8))*out;

    short reverb_value = 0;
    /*
    reverb_value += 0.2f*m->reverb_buffer[(44100+m->reverb_buffer_tracker-1447)%44100];
    reverb_value += 0.3f*m->reverb_buffer[(44100+m->reverb_buffer_tracker-587)%44100];
    reverb_value += 0.2f*m->reverb_buffer[(44100+m->reverb_buffer_tracker-3881)%44100];
    reverb_value += 0.1f*m->reverb_buffer[(44100+m->reverb_buffer_tracker-6047)%44100];
    return_value += reverb_value;
    */

    //return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+1)%44100];
    //return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+22050+1)%44100];
    /*
    return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+44100+1 - 233)%44100];
    return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+44100+1 - 1297)%44100];
    return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+44100+1 - 2621)%44100];
    return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+44100+1 - 3581)%44100];
    return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+44100+1 - 5449)%44100];
    */


    return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+44100+1 - 7001)%44100];
    return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+44100+1 - 10009)%44100];
    return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+44100+1 - 13997)%44100];
    return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+44100+1 - 17099)%44100];
    return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+44100+1 - 21089)%44100];
    return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+44100+1 - 29173)%44100];
    return_value += 0.1f*m->reverb_buffer[(m->reverb_buffer_tracker+44100+1 - 37957)%44100];
    return_value = (
                    0.4f*return_value +
                    0.3f*m->reverb_buffer[(m->reverb_buffer_tracker-1)%44100] +
                    0.2f*m->reverb_buffer[(m->reverb_buffer_tracker-2)%44100] +
                    0.1f*m->reverb_buffer[(m->reverb_buffer_tracker-3)%44100]
                   );

    m->reverb_buffer[m->reverb_buffer_tracker] = return_value;
    m->reverb_buffer_tracker = (m->reverb_buffer_tracker+1)%44100;
    return return_value;
}

void mixer_add_instrument(Mixer* m, Instrument* in){
    if(m->number_of_instruments < 16){
        m->instruments[m->number_of_instruments++] = in;
    }
}
