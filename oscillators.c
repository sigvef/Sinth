#include <math.h>

float osc_sin(float pitch,int time){
    return sin(time*M_PI/44100 *440*pow(2,(pitch-57)/12));
}

float osc_squ(float pitch, int time){
    return (osc_sin(pitch,time)+(osc_sin(pitch-12,time)<0 ? 1 : -1))*0.5f;
}
