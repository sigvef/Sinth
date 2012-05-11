#ifndef OSCILLATORS_H
#define OSCILLATORS_H

#define OSC_SIN 0
#define OSC_SQU 1
#define OSC_SAW 2
#define OSC_TRI 3
#define OSC_NOISE 4
#define OSC_SILENCE 5

float sine(float x);
float osc_sin(float pitch, int time);
float osc_squ(float pitch, int time);
float osc_saw(float pitch, int time);
float osc_tri(float pitch, int time);

#endif
