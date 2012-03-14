#ifndef VOICE_H
#define VOICE_H

typedef struct{
    float pitch;
    float volume;
    char on;
    unsigned char active;
    unsigned int tracker;
    float adsr_modifier;
} Voice;


#endif
