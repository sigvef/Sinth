#include <math.h>

float sine(float x){

    x = x - ceil(x/M_PI)*M_PI;
    const float B = 4/M_PI;
    const float C = -4/(M_PI*M_PI);

    return -(B*x + C*x*fabsf(x));
}

float osc_sin(float pitch,int time){
    return sin(time*M_PI/44100 *440*pow(2,(pitch-57-12-12)/12));
}

float osc_squ(float pitch, int time){
    return (osc_sin(pitch,time)<0 ? 1 : -1);
}

float osc_saw(float pitch, int time){
    int freq = 44100/(440*pow(2,(pitch-57-12-12)/12));
    return (time%freq) / (float)(freq) -0.5;
}

float osc_tri(float pitch, int time){
    int freq = 44100.f/(440*pow(2,(pitch-57-12-12)/12));
    if(!freq) freq=1;
    float v = time%freq;
    v = 4*v/freq;
    return v<0.5 ? 1-v : v-3;
}


